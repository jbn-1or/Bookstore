#pragma once

#include <string>

struct Transaction {
    std::string time;          // 时间戳（YYYY-MM-DD HH:MM:SS）
    std::string type;          // "buy"或"import"
    std::string isbn;
    int quantity;              // 交易数量（>0）
    double amount;             // 金额（buy为正，import为负）
    std::string operator_id;   // 操作人ID

    Transaction() = default;
    Transaction(const std::string& t, const std::string& typ, const std::string& isbn,
                int qty, double amt, const std::string& op);
    std::string to_string() const;  // 转换为存储格式字符串
};