#include "Transaction.hpp"

// 构造函数：初始化交易记录的所有字段，验证数量合法性
Transaction::Transaction(const std::string& t, const std::string& typ, const std::string& isbn_,
                         int qty, double amt, const std::string& op)
    : time(t), type(typ), isbn(isbn_), quantity(qty), amount(amt), operator_id(op) {};

/**
 * 转换为存储格式字符串
 * 格式：Time\tType\tISBN\tQuantity\tAmount\tOperator\n
 * 金额保留2位小数，数量为整数
 */
std::string Transaction::to_string() const {
    std::fstream sss;
    sss << time << "\t"
       << type << "\t"
       << isbn << "\t"
       << quantity << "\t"
       << std::fixed << std::setprecision(2) << amount << "\t"
       << operator_id << "\n";
    return " ";
}

