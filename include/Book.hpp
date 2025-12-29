#pragma once

#include "Storage.hpp"
#include <iomanip>
#include <string>

class Book {
public:
    char isbn[MAX_INDEX_LEN];          // 唯一标识
    char name[MAX_INDEX_LEN] = {};          // 书名
    char author[MAX_INDEX_LEN] = {};        // 作者
    char keyword[MAX_INDEX_LEN] = {};       // 关键词
    int price_up = 0;              // 单价高位
    int price_down = 0;            // 乘100后最低八位
    int quantity = 0;              // 库存数量（≥0）

    Book() = default;
    Book(const std::string& isbn);  // 仅初始化ISBN（用于select创建）
    Book(const std::string& isbn, const std::string& name, const std::string& author, 
         const std::string& keyword, double price, int quantity);

    bool operator<(const Book &other) const;
    bool operator== (const Book& other) const;
    std::string get_index() const;
    // 库存操作
    bool increase_quantity(int num);  // 进货
    bool decrease_quantity(int num);  // 购买
    // 修改属性（返回是否成功）
    bool set_name(const std::string& new_name);
    bool set_author(const std::string& new_author);
    bool set_keyword(const std::string& new_keyword);
    bool set_price(const long long& new_price);
    bool set_isbn(const std::string& new_isbn);
};

//重载流运算 <<
inline std::ostream & operator<< (std::ostream &os, const Book& book) {
     long long PRC = book.price_up * 100000000 + book.price_down;
     int price_int = PRC / 100;
     int price_dec = PRC % 100;
     os << std::string(book.isbn) << "\t"
        << std::string(book.name) << "\t"
        << std::string(book.author) << "\t"
        << std::string(book.keyword) << "\t"
        << price_int << "." << std::setw(2) << std::setfill('0') << price_dec << "\t"
        << std::resetiosflags(std::ios::adjustfield) << book.quantity << "\n";
     return os;
}

bool load_book(const std::string& book_id, Book& _book);
bool find_book(const std::string &bool_id);
bool print_book(const std::string &book_id);
bool delete_isbn(const std::string& book_id); 
void update_book(const Book & _book);
bool show_all();