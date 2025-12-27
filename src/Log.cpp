#include "Log.hpp"
#include <sstream>

// 构造函数：初始化日志的所有字段
Log::Log(const std::string& t, const std::string& op_id, const std::string& op, const std::string& det)
    : time(t), operator_id(op_id), operation(op), details(det) {}

/**
 * 转换为存储格式字符串
 * 格式：Time\tOperator\tOperation\tDetails\n
 * 时间戳格式为YYYY-MM-DD HH:MM:SS，其他字段按原始值存储
 */
std::string Log::to_string() const {
    std::stringstream ss;
    ss << time << "\t"
       << operator_id << "\t"
       << operation << "\t"
       << details << "\n";
    return ss.str();
}
