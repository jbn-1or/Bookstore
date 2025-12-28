#pragma once

#include "Storage.hpp"
#include <string>

//检查字符情况的类
bool checkUserID_or_Password(const std::string &);
bool checkUsername(const std::string &);
bool checkPrivilege(const std::string &);
bool checkISBN(const std::string &);
bool checkBookName_or_Author(const std::string &);
bool checkKeyword(const std::string &);
bool checkQuantity(const std::string &, int &);
bool checkPrice_or_TotalCost(const std::string &, long long &);


//查找的辅助类
class KeyW_to_ISBN {
    public:
    char keyW[MAX_INDEX_LEN];
    char ISBN[MAX_INDEX_LEN];
    
    KeyW_to_ISBN(const std::string & kw, const std::string & i);
    std::string get_index() const;
    bool operator< (const KeyW_to_ISBN & other) const;
    bool operator== (const KeyW_to_ISBN & other) const;
};

bool findkeyW(const std::string & kw, std::vector<std::string> ISBNs);
void updateKI(const KeyW_to_ISBN & ki);
void deleteKI(const KeyW_to_ISBN & ki);

class Name_to_ISBN {
    public:
    char Name[MAX_INDEX_LEN];
    char ISBN[MAX_INDEX_LEN];
    
    Name_to_ISBN(const std::string & na, const std::string & i);
    std::string get_index() const;
    bool operator< (const Name_to_ISBN & other) const;
    bool operator== (const Name_to_ISBN & other) const;
};

bool findName(const std::string & na, std::vector<std::string> ISBNs);
void updateNI(const Name_to_ISBN & ni);
void deleteNI(const Name_to_ISBN & ni);

class Author_to_ISBN {
    public:
    char Author[MAX_INDEX_LEN];
    char ISBN[MAX_INDEX_LEN];
    Author_to_ISBN(const std::string & kw, const std::string & i);
    std::string get_index() const;
    bool operator< (const Author_to_ISBN & other) const;
    bool operator== (const Author_to_ISBN & other) const;
};

bool findAuthor(const std::string & au, std::vector<std::string> ISBNs);
void updateAI(const Author_to_ISBN & ai);
void deleteAI(const Author_to_ISBN & ai);
