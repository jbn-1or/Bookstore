#include "Utils.hpp"
#include "Storage.hpp"

#include <cctype>
#include <string>
#include <vector>

bool checkUserID_or_Password(const std::string & input) {
    if (input.size() > 30) {
        return false;
    }
    for (char c : input) {
        if (!std::isalnum(static_cast<char>(c)) || c == '_') { //数字字母下划线
            return false;
        }
    }
    return true;
}

bool checkUsername(const std::string & input) {
    if (input.size() > 30) {
        return false;
    }
    for (char c : input) {
        if (!std::isprint(static_cast<char>(c))) { //可见字符
            return false;
        }
    }
    return true;
}

bool checkPrivilege(const std::string & input) {
    if (input.size() > 1) {
        return false;
    }
    for (char c : input) {
        int num = static_cast<char>(c) - '0';
        if (!(num == 1 || num == 3 || num ==7)) { //数字
            return false;
        }
    }
    return true;
}

bool checkISBN(const std::string & input) {
    if (input.size() > 20) {
        return false;
    }
    for (char c : input) {
        if (!std::isprint(static_cast<char>(c))) { //可见字符
            return false;
        }
    }
    return true;
}

bool checkBookName_or_Author(const std::string & input) {
    if (input.size() > 60) {
        return false;
    }
    for (char c : input) {
        char ch = static_cast<char>(c);
        if (! ((ch >= 33 && ch <= 126) && (ch != '"'))) {
            return false;
        }
    }
    return true;
}

bool checkKeyword(const std::string & input) {
    if (input.size() > 60) {
        return false;
    }
    for (char c : input) {
        char ch = static_cast<char>(c);
        if (! ((ch >= 33 && ch <= 126) && (ch != '"'))) {
            return false;
        }
    }
    return true;
}

bool checkQuantity(const std::string & input, int & num) {
    if (input.size() > 10) {
        return false;
    }
    num = 0;
    for (char c : input) {
        char ch = static_cast<char>(c) - '0';
        if (! (ch >= 0 && ch <= 9)) {
            return false;
        }
        num = num * 10 + ch;
    }
    return true;
}

bool checkPrice_or_TotalCost(const std::string & input, long long & num) {
    if (input.size() > 13) {
        return false;
    }
    num = 0;
    bool hasDot = false;
    int digitsAfterDot = 0;

    for (char c : input) {
        if (c == '.') {
            if (hasDot) return false; // 只能有一个小数点
            hasDot = true;
            continue;
        }
        if (!std::isdigit(static_cast<char>(c))) {
            return false; // 非数字且非小数点
        }
        int digit = c - '0';
        if (!hasDot) {
            // 小数点前的部分
            num = num * 10 + digit;
        } else {
            // 小数点后的部分，最多两位
            if (digitsAfterDot < 2) {
                num = num * 10 + digit;
                digitsAfterDot++;
            }
        }
    }
    // 如果没有小数点乘以100
    if (!hasDot) {
        num *= 100;
    }
    // 如果有小数点但不足两位，补零
    else {
        while (digitsAfterDot < 2) {
            num *= 10;
            digitsAfterDot++;
        }
    }
    return true;
}

BlockListManager<KeyW_to_ISBN> KIManager("Keyword-ISBN.data", MAX_INDEX_LEN, MAX_ENTRIES_PER_BLOCK);

KeyW_to_ISBN::KeyW_to_ISBN(const std::string & kw, const std::string & i) {
    std::strncpy(keyW, kw.c_str(), MAX_INDEX_LEN - 1);
    keyW[MAX_INDEX_LEN - 1] = '\0';
    std::strncpy(ISBN, i.c_str(), MAX_INDEX_LEN - 1);
    ISBN[MAX_INDEX_LEN - 1] = '\0';
}

std::string KeyW_to_ISBN::get_index() const {
    return std::string(keyW);
}

bool KeyW_to_ISBN::operator< (const KeyW_to_ISBN & other) const {
    return this->get_index() < other.get_index();
}
bool KeyW_to_ISBN::operator== (const KeyW_to_ISBN & other) const {
    std::string tmp1(this->ISBN);
    std::string tmp2(other.ISBN);
    if (this->get_index() == other.get_index() && tmp1 == tmp2) {
        return true;
    } else {
        return false;
    }
}

bool findkeyW(const std::string & kw, std::vector<std::string> &ISBNs) {
    std::vector<KeyW_to_ISBN> KIs;
    ISBNs.clear();
    KIs.clear();
    KIManager.findEntry(kw, KIs);
    for (int i = 0; i < KIs.size(); ++i) {
        ISBNs.push_back(KIs[i].ISBN);
    }
    return !KIs.empty();
}

void updateKI(const KeyW_to_ISBN & ki) {
    KIManager.insertEntry(ki);
}

void deleteKI(const KeyW_to_ISBN & ki) {
    KIManager.deleteEntry(ki);
}

BlockListManager<Name_to_ISBN> NIManager("Name-ISBN.data", MAX_INDEX_LEN, MAX_ENTRIES_PER_BLOCK);


Name_to_ISBN::Name_to_ISBN(const std::string &na, const std::string &i) {
    std::strncpy(Name, na.c_str(), MAX_INDEX_LEN - 1);
    Name[MAX_INDEX_LEN - 1] = '\0';
    std::strncpy(ISBN, i.c_str(), MAX_INDEX_LEN - 1);
    ISBN[MAX_INDEX_LEN - 1] = '\0';
}

std::string Name_to_ISBN::get_index() const {
    return std::string(Name);
}

bool Name_to_ISBN::operator< (const Name_to_ISBN &other) const {
    return this->get_index() < other.get_index();
}

bool Name_to_ISBN::operator== (const Name_to_ISBN &other) const {
    std::string tmp1(this->ISBN);
    std::string tmp2(other.ISBN);
    if (this->get_index() == other.get_index() && tmp1 == tmp2) {
        return true;
    } else {
        return false;
    }
}

bool findName(const std::string &na, std::vector<string> &ISBNs) {
    std::vector<KeyW_to_ISBN> NIs;
    ISBNs.clear();
    NIs.clear();
    KIManager.findEntry(na, NIs);
    for (int i = 0; i < NIs.size(); ++i) {
        ISBNs.push_back(NIs[i].ISBN);
    }
    return !NIs.empty();
}

void updateNI(const Name_to_ISBN &ni) {
    NIManager.insertEntry(ni);
}

void deleteNI(const Name_to_ISBN &ni) {
    NIManager.deleteEntry(ni);
}

BlockListManager<Author_to_ISBN> AIManager("Author-ISBN.data", MAX_INDEX_LEN, MAX_ENTRIES_PER_BLOCK);

Author_to_ISBN::Author_to_ISBN(const std::string &au, const std::string &i) {
    std::strncpy(Author, au.c_str(), MAX_INDEX_LEN - 1);
    Author[MAX_INDEX_LEN - 1] = '\0';
    std::strncpy(ISBN, i.c_str(), MAX_INDEX_LEN - 1);
    ISBN[MAX_INDEX_LEN - 1] = '\0';
}

std::string Author_to_ISBN::get_index() const {
    return std::string(Author);
}

bool Author_to_ISBN::operator< (const Author_to_ISBN &other) const {
    return this->get_index() < other.get_index();
}

bool Author_to_ISBN::operator== (const Author_to_ISBN &other) const {
    std::string tmp1(this->ISBN);
    std::string tmp2(other.ISBN);
    if (this->get_index() == other.get_index() && tmp1 == tmp2) {
        return true;
    } else {
        return false;
    }
}

bool findAuthor(const std::string &au, std::vector<string> &ISBNs) {
    std::vector<KeyW_to_ISBN> AIs;
    ISBNs.clear();
    AIs.clear();
    KIManager.findEntry(au, AIs);
    for (int i = 0; i < AIs.size(); ++i) {
        ISBNs.push_back(AIs[i].ISBN);
    }
    return !AIs.empty();
}

void updateAI(const Author_to_ISBN &ai) {
    AIManager.insertEntry(ai);
}

void deleteAI(const Author_to_ISBN &ai) {
    AIManager.deleteEntry(ai);
}