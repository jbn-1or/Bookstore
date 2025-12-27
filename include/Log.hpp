#pragma once

#include <string>
#include <vector>

struct Log {
    std::string time;          // 时间戳
    std::string operator_id;   // 操作人ID（未登录为"guest"）
    std::string operation;     // 操作类型（如"login"、"buy"等）
    std::string details;       // 操作详情

    Log() = default;
    Log(const std::string& t, const std::string& op_id, const std::string& op, const std::string& det);
    std::string to_string() const;  // 转换为存储格式字符串
};