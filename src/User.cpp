#include "User.hpp"

BlockListManager<User> UserDataManager("User.data", MAX_INDEX_LEN, MAX_ENTRIES_PER_BLOCK);


// User类实现
User::User(const std::string& id, const std::string& pwd, int priv,
    const std::string& name) : privilege(priv) {

        std::strncpy(user_id, id.c_str(), MAX_INDEX_LEN - 1);
        user_id[MAX_INDEX_LEN - 1] = '\0';
        std::strncpy(password, pwd.c_str(), MAX_INDEX_LEN - 1);
        password[MAX_INDEX_LEN - 1] = '\0';
        std::strncpy(username, name.c_str(), MAX_INDEX_LEN - 1);
        username[MAX_INDEX_LEN - 1] = '\0';
    }

bool User::operator<(const User &other) const {
    return this->get_index() < other.get_index();
}

bool User::operator==(const User &other) const {
    return this->get_index() == other.get_index();
}

std::string User::get_index() const {
    return std::string(user_id);
}

std::string User::get_user_id() const {
    return std::string(user_id);
}

std::string User::get_username() const {
    return std::string(username);
}

int User::get_privilege() const {
    return privilege;
}

bool User::verify_password(const std::string& input) const {
    return password == input;
}

bool User::has_privilege(int required) const {
    return privilege >= required;
}

void User::set_password(const std::string& new_pwd) {
    std::strncpy(password, new_pwd.c_str(), MAX_INDEX_LEN - 1);
    password[MAX_INDEX_LEN - 1] = '\0';
}

// UserManager类实现
bool UserManager::find_user(const std::string &user_id) {
    std::vector<User> target;
    UserDataManager.findEntry(user_id, target);
    return !target.empty();
}

bool UserManager::load_user(const std::string& user_id, User& user) {
    std::vector<User> target;
    UserDataManager.findEntry(user_id, target);
    if (target.size() == 0) {
        return false;
    }
    user = target[0];
    return true;
}

void UserManager::save_user(const User& user) {
    UserDataManager.insertEntry(user);
}

bool UserManager::delete_user_file(const std::string& user_id) {
    std::vector<User> target;
    UserDataManager.findEntry(user_id, target);
    if (target.size() == 0) {
        return false;
    }
    User tmp = target[0];
    UserDataManager.deleteEntry(tmp);
    return true;
}

void UserManager::init_root() {
    // 检查是否已存在root用户
    if (find_user("root")) {
        return;
    }
    // 创建root用户（权限7）
    User new_root("root", "sjtu", 7, "ASUKA");
    save_user(new_root);
    return;
}

void UserManager::select_book(const Book & bk) {
    select_book_stack.pop_back();
    select_book_stack.push_back(bk);
    has_selected.back() = true;
}

bool UserManager::get_select_book(Book &bk) {
    if (!select_book_stack.empty()) {
        bk = select_book_stack.back();
        return true;
    } else return false;
}

void UserManager::update_book_stack(const Book & bk, const Book & newbook) {
    for (int i = 0; i < select_book_stack.size(); ++i) {
        if (select_book_stack[i] == bk) {
            select_book_stack[i] = newbook;
        }
    }
}

bool UserManager::register_user(const std::string& user_id, const std::string& pwd, const int priv,const std::string& name) {
    // 检查用户是否已存在
    if (find_user(user_id)) {
        return false;
    }
    User user(user_id, pwd, priv, name);
    save_user(user);
    return true;
}

bool UserManager::login(const std::string& user_id, const std::string& pwd) {
    User user;
    if (!load_user(user_id, user)) {
        return false;
    }
    if ((!login_stack.empty() && login_stack.back().get_privilege() <= user.get_privilege()) && !user.verify_password(pwd)) {
        return false;
    }
    login_stack.push_back(user);
    select_book_stack.push_back(book0);
    has_selected.push_back(false);
    return true;
}

bool UserManager::logout() {
    if (login_stack.empty()) {
        return false;
    }
    login_stack.pop_back();
    select_book_stack.pop_back();
    has_selected.pop_back();
    return true;
}

bool UserManager::create_user(const std::string& user_id, const std::string& pwd, 
                            int priv, const std::string& name) {
    // 权限检查：创建者权限必须高于被创建者
    if (priv >= this->get_current_privilege()) {
        return false;
    }
    // 检查用户是否已存在
    if (find_user(user_id)) {
        return false;
    }
    User new_user(user_id, pwd, priv, name);
    save_user(new_user);
    return true;
}

bool UserManager::delete_user(const std::string& user_id) {
    // 仅最高权限(7)可删除用户
    if (this->get_current_privilege() != 7) {
        return false;
    }
    // 检查用户是否存在
    User user;
    if (!load_user(user_id, user)) {
        return false;
    }
    // 检查用户是否已登录
    for (const auto& u : login_stack) {
        if (u.get_user_id() == user_id) {
            return false;
        }
    }
    return delete_user_file(user_id);
}

bool UserManager::change_password(const std::string& user_id, const std::string& old_pwd, 
                                 const std::string& new_pwd) {
    User user;
    if (!load_user(user_id, user)) {
        return false;
    }
    // 最高权限可跳过旧密码验证
    if (this->get_current_privilege() != 7 && !user.verify_password(old_pwd)) {
        return false;
    }
    delete_user_file(user_id);
    user.set_password(new_pwd);
    save_user(user);
    // 更改栈上的用户
    for (int i = 0; i < login_stack.size(); ++i) {
        if (login_stack[i].get_index() == user_id) {
            login_stack[i] = user;
        }
    }
    return true;
}

int UserManager::get_current_privilege() const {
    if (login_stack.empty()) {
        return 0;
    }
    return login_stack.back().get_privilege();
}

std::string UserManager::get_current_user_id() const {
    if (login_stack.empty()) {
        return "guest";
    }
    return login_stack.back().get_user_id();
}