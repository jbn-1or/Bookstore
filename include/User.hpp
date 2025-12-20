#pragma once

#include <string>
#include <vector>

class User {
private:
    std::string user_id;       // 登录ID（唯一）
    std::string password;      // 密码
    int privilege;             // 权限等级（1/3/7）
    std::string username;      // 用户名
public:
    User() = default;
    User(const std::string& id, const std::string& pwd, int priv, const std::string& name);
    std::string get_user_id() const;
    std::string get_username() const;
    int get_privilege() const;
    bool verify_password(const std::string& input) const;  // 密码验证
    bool has_privilege(int required) const;                // 权限检查（≥required）
    void set_password(const std::string& new_pwd);         // 修改密码
};

class UserManager {
private:
    std::string file_path;                 // 数据文件路径
    std::vector<User> login_stack;          // 嵌套登录栈
    bool load_user(const std::string& user_id, User& user);  // 从文件加载单个用户
    bool save_user(const User& user);      // 保存用户到文件
    bool delete_user_file(const std::string& user_id);  // 从文件删除用户
public:
    UserManager(const std::string& path);
    bool init_root();                      // 初始化root用户（首次运行）
    bool register_user(const std::string& user_id, const std::string& pwd, const std::string& name);  // {0}可调用
    bool login(const std::string& user_id, const std::string& pwd);
    bool logout();
    bool create_user(const std::string& user_id, const std::string& pwd, 
        int priv, const std::string& name, int current_priv);  // 权限检查
    bool delete_user(const std::string& user_id, int current_priv);  // 权限检查
    bool change_password(const std::string& user_id, const std::string& old_pwd, const std::string& new_pwd, int current_priv);
    int get_current_privilege() const;     // 获取当前登录用户权限
    std::string get_current_user_id() const;  // 获取当前登录用户ID
};