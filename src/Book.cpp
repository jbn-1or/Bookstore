#include "Book.hpp"
#include "Storage.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <string>

BlockListManager<Book> BookDataManager("Book.data", MAX_INDEX_LEN, MAX_ENTRIES_PER_BLOCK);

// 构造函数实现
Book::Book(const std::string& i) : price_up(0), price_down(0) , quantity(0) {
    std::strncpy(isbn, i.c_str(), MAX_INDEX_LEN - 1);
    isbn[MAX_INDEX_LEN - 1] = '\0';
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
    update_book(*this);
    return true;
}

bool Book::decrease_quantity(int num) {
    if (num <= 0 || quantity < num) return false;
    quantity -= num;
    update_book(*this);
    return true;
}

// 属性修改
bool Book::set_name(const std::string& new_name) {
    if (new_name.empty()) {
        return false;
    }
    //删NI
    std::string i(isbn);
    std::string na(name);
    Name_to_ISBN NI(na, i);
    deleteNI(NI);
    //添新NI
    Name_to_ISBN NI_(new_name, i);
    updateNI(NI_);

    //改书
    std::strncpy(name, new_name.c_str(), MAX_INDEX_LEN - 1);
    name[MAX_INDEX_LEN - 1] = '\0';
    update_book(*this);
    return true;
}

bool Book::set_author(const std::string& new_author) {
    if (new_author.empty()) {
        return false;
    }
    if (new_author == std::string(author)) {
        return true;
    }
    //删AI
    std::string i(isbn);
    Author_to_ISBN oldAI(author, i);
    deleteAI(oldAI);
    //添新AI
    Author_to_ISBN newAI(new_author, i);
    updateAI(newAI);

    //改书
    std::strncpy(author, new_author.c_str(), MAX_INDEX_LEN - 1);
    author[MAX_INDEX_LEN - 1] = '\0';
    update_book(*this);
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
    // 处理最后一个关键词
    if (!temp.empty()) {
        if (std::find(keywords.begin(), keywords.end(), temp) != keywords.end()) {
            return false;
        }
        keywords.push_back(temp);
    }

    // 修改KW-ISBN文件
      //删
    if (keyword[0] != '\0') {
        std::string kw_str(keyword);
        size_t pos = 0;
        while ((pos = kw_str.find('|')) != std::string::npos) {
            std::string x = kw_str.substr(0, pos);
            KeyW_to_ISBN KI(x, std::string(isbn));
            deleteKI(KI);
            kw_str.erase(0, pos + 1);
        }
        // 最后一个关键词
        if (!kw_str.empty()) {
            KeyW_to_ISBN KI(kw_str, std::string(isbn));
            deleteKI(KI);
        }
    }
        //添加
        for (int i = 0; i < keywords.size(); ++i) {
            KeyW_to_ISBN KI(keywords[i], std::string(isbn));
            updateKI(KI);
        }

    // 修改当前书
    std::strncpy(keyword, new_keyword.c_str(), MAX_INDEX_LEN - 1);
    keyword[MAX_INDEX_LEN - 1] = '\0';
    update_book(*this);
    return true;
}

bool Book::set_price(const long long & newp) {
    if (newp < 0) return false;
    price_up = newp / 10000000;
    price_down = newp % 10000000;
    update_book(*this);
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
    // 修改所有KI，NI，AI
    ////修改KI
    if (keyword[0] != '\0') {
        std::string kw_str(keyword);
        size_t pos = 0;
        while ((pos = kw_str.find('|')) != std::string::npos) {
            std::string x = kw_str.substr(0, pos);
            KeyW_to_ISBN KI(x, std::string(isbn));
            KeyW_to_ISBN KI_(x, std::string(new_isbn));
            deleteKI(KI);
            updateKI(KI_);
            kw_str.erase(0, pos + 1);
        }
        // 最后一个关键词
        if (!kw_str.empty()) {
            KeyW_to_ISBN KI(kw_str, std::string(isbn));
            deleteKI(KI);
            KeyW_to_ISBN KI_(kw_str, std::string(new_isbn));
            updateKI(KI_);
        }
    }
    ////修改NI
    if (name[0] != '\0') {
        std::string na(name);
        Name_to_ISBN NI(na, std::string(isbn));
        deleteNI(NI);
        Name_to_ISBN NI_(na, std::string(new_isbn));
        updateNI(NI_);
    }
    ////修改AI
    if (author[0] != '\0') {
        std::string au(author);
        Author_to_ISBN AI(au, std::string(isbn));
        deleteAI(AI);
        Author_to_ISBN AI_(au, std::string(new_isbn));
        updateAI(AI_);
    }
    //改书
    delete_isbn(std::string(this->isbn)); //删除旧书
    std::strncpy(isbn, new_isbn.c_str(), MAX_INDEX_LEN - 1);
    isbn[MAX_INDEX_LEN - 1] = '\0';
    update_book(*this);
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

bool print_book(const std::string &book_id) {
    std::vector<Book> target = {};
    BookDataManager.findEntry(book_id, target);
    if (target.size() == 1) {
        std::cout << target[0];
        return true;
    } else return false;
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

bool show_all() {
    if (BookDataManager.printAllEntries()) {
        return true;
    } else {
        return false;
    }
}
