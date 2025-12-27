#include "Book.hpp"
#include "Storage.hpp"
#include <sstream>
#include <algorithm>

// 构造函数实现
Book::Book(const std::string& i) : price(0.0), quantity(0) {
    std::strncpy(isbn, i.c_str(), MAX_INDEX_LEN - 1);
    isbn[MAX_INDEX_LEN - 1] = '\0';
}

Book::~Book() {
    update_book(*this);
}

bool Book::operator<(const Book &other) const {
    return this->get_index() < other.get_index();
}

bool Book::operator==(const Book &other) const {
    return this->get_index() == other.get_index();
}

std::string Book::get_index() const {
    return std::string(isbn);
}

// 库存操作
bool Book::increase_quantity(int num) {
    if (num <= 0) return false;
    quantity += num;
    return true;
}

bool Book::decrease_quantity(int num) {
    if (num <= 0 || quantity < num) return false;
    quantity -= num;
    return true;
}

// 属性修改
bool Book::set_name(const std::string& new_name) {
    if (new_name.empty()) return false;
    std::strncpy(name, new_name.c_str(), MAX_INDEX_LEN - 1);
    name[MAX_INDEX_LEN - 1] = '\0';
    return true;
}

bool Book::set_author(const std::string& new_author) {
    if (new_author.empty()) return false;
    std::strncpy(author, new_author.c_str(), MAX_INDEX_LEN - 1);
    author[MAX_INDEX_LEN - 1] = '\0';
    return true;
}

bool Book::set_keyword(const std::string& new_keyword) {
    // 检查关键词是否有重复
    std::string temp = new_keyword;
    size_t pos = 0;
    std::vector<std::string> keywords;
    while ((pos = temp.find('|')) != std::string::npos) {
        std::string kw = temp.substr(0, pos);
        if (std::find(keywords.begin(), keywords.end(), kw) != keywords.end()) {
            return false; // 存在重复关键词
        }
        keywords.push_back(kw);
        temp.erase(0, pos + 1);
    }
    if (!temp.empty() && std::find(keywords.begin(), keywords.end(), temp) != keywords.end()) {
        return false;
    }
    std::strncpy(keyword, new_keyword.c_str(), MAX_INDEX_LEN - 1);
    keyword[MAX_INDEX_LEN - 1] = '\0';
    return true;
}

bool Book::set_price(double new_price) {
    if (new_price < 0) return false;
    price = new_price;
    return true;
}

bool Book::set_isbn(const std::string& new_isbn) {
    if (new_isbn.empty() || new_isbn == isbn) {
        return false;
    }
    // 检查新ISBN是否已存在
    if (find_book(new_isbn)) {
        return false;
    }
    delete_isbn(std::string(this->isbn));
    std::strncpy(isbn, new_isbn.c_str(), MAX_INDEX_LEN - 1);
    isbn[MAX_INDEX_LEN - 1] = '\0';
    return true;
}

bool load_book(const std::string& book_id, Book& _book) {
    std::vector<Book> target;
    BookDataManager.findEntry(book_id, target);
    if (target.size() == 0) {
        return false;
    }
    _book = target[0];
    return true;
}

bool find_book(const std::string &book_id) {
    std::vector<Book> target;
    BookDataManager.findEntry(book_id, target);
    return !target.empty();
}

bool delete_isbn(const std::string& book_id) {
    std::vector<Book> target;
    BookDataManager.findEntry(book_id, target);
    if (target.size() == 0) {
        return false;
    }
    Book tmp = target[0];
    BookDataManager.deleteEntry(tmp);
    return true;
}

void update_book(const Book &_book) {
    delete_isbn(std::string(_book.isbn));
    BookDataManager.insertEntry(_book);
    return;
}