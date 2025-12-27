#pragma once

#include "Book.hpp"
#include "Storage.hpp"
#include <vector>
#include <string>

class User {
private:
    char user_id[MAX_INDEX_LEN];       // 登录ID（唯一）
    char password[MAX_INDEX_LEN];      // 密码
    int privilege = 1;                 // 权限等级（0/1/3/7)
    char username[MAX_INDEX_LEN];      // 用户名
public:
    User() = default;
    User(const std::string& id, const std::string& pwd, int priv, const std::string& name);
    bool operator<(const User &other) const;
    bool operator== (const User& other) const;
    std::string get_index() const;
    std::string get_user_id() const;
    std::string get_username() const;
    int get_privilege() const;
    bool verify_password(const std::string& input) const;  // 密码验证
    bool has_privilege(int required) const;                // 权限检查（≥required）
    void set_password(const std::string& new_pwd);         // 修改密码
};

//重载流函数（默认）
inline std::ostream & operator<< (std::ostream &os, const User& user) {
    os << user.get_user_id();
    return os;
}

class UserManager {
private:
    //std::string file_path;                 // 数据文件路径
    std::vector<User> login_stack;          // 嵌套登录栈
    std::vector<Book> select_book_stack;    // 对应选择的书
    bool find_user(const std::string &user_id);
    bool load_user(const std::string& user_id, User& user);  // 从文件加载单个用户
    void save_user(const User& user);      // 保存用户到文件
    bool delete_user_file(const std::string& user_id);  // 从文件删除用户
public:
    UserManager() = default;
    void init_root();                      // 初始化root用户（首次运行）
    bool register_user(const std::string& user_id, const std::string& pwd, const int priv ,const std::string& name);
    bool login(const std::string& user_id, const std::string& pwd);
    bool logout();
    bool create_user(const std::string& user_id, const std::string& pwd, 
        int priv, const std::string& name);
    bool delete_user(const std::string& user_id);
    bool change_password(const std::string& user_id, const std::string& old_pwd, const std::string& new_pwd);
    int get_current_privilege() const;     // 获取当前登录用户权限
    std::string get_current_user_id() const;  // 获取当前登录用户ID
};