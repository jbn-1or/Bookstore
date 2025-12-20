#pragma once

#include <string>

class Book {
private:
    std::string isbn;          // 唯一标识
    std::string name;          // 书名
    std::string author;        // 作者
    std::string keyword;       // 关键词（|分隔，无重复）
    double price;              // 单价（保留2位小数）
    int quantity;              // 库存数量（≥0）
public:
    Book() = default;
    Book(const std::string& isbn);  // 仅初始化ISBN（用于select创建）
    Book(const std::string& isbn, const std::string& name, const std::string& author, 
         const std::string& keyword, double price, int quantity);
    // get
    std::string get_isbn() const;
    std::string get_name() const;
    int get_quantity() const;
    double get_price() const;
    // 库存操作
    bool increase_quantity(int num);  // 进货
    bool decrease_quantity(int num);  // 购买
    // 修改属性（返回是否成功）
    bool set_name(const std::string& new_name);
    bool set_author(const std::string& new_author);
    bool set_keyword(const std::string& new_keyword);
    bool set_price(double new_price);
    bool set_isbn(const std::string& new_isbn);
    // 匹配查询条件
    bool match(const std::string& type, const std::string& value) const;
};