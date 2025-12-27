#pragma once

#include <string>

#include "Book.hpp"
#include <iomanip>

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

// 交易管理器类实现（负责交易记录的读写与统计）
class TransactionManager {
    bool record_transaction(const Transaction& trans) {
        return true;
    }

    bool query_transactions(const std::string& type, const std::string& start_time, const std::string& end_time) {
        return true;
    }

    bool calculate_profit(const std::string& start_time, const std::string& end_time, double& income, double& expense, double& profit) {
        income = 0.0;
        expense = 0.0;
        profit = 0.0;
        profit = income - expense;
        return true;
    }
};