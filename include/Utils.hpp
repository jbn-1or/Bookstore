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
bool checkQuantity(const std::string &);
bool checkPrice_or_TotalCost(const std::string &);


//查找的辅助类
class KeyW_to_ISBN {
    public:
    char keyW[MAX_INDEX_LEN];
    char ISBN[MAX_INDEX_LEN];
    
    KeyW_to_ISBN(const std::string & kw, const std::string & i);
    ~KeyW_to_ISBN();
    std::string get_index() const;
    bool operator< (const KeyW_to_ISBN & other);
    bool operator== (const KeyW_to_ISBN & other);
};

bool findkeyW(const std::string & kw);
bool updateKI(const KeyW_to_ISBN & ki);
bool deleteKI(const KeyW_to_ISBN & ki);

class Name_to_ISBN {
    public:
    char Name[MAX_INDEX_LEN];
    char ISBN[MAX_INDEX_LEN];
    
    Name_to_ISBN(const std::string & na, const std::string & i);
    ~Name_to_ISBN();
    std::string get_index() const;
    bool operator< (const Name_to_ISBN & other);
    bool operator== (const Name_to_ISBN & other);
};

bool findName(const std::string & na);
bool updateNI(const Name_to_ISBN & ni);
bool deleteNI(const Name_to_ISBN & ni);

class Author_to_ISBN {
    public:
    char Auther[MAX_INDEX_LEN];
    char ISBN[MAX_INDEX_LEN];
    Author_to_ISBN(const std::string & kw, const std::string & i);
    ~Author_to_ISBN();
    std::string get_index() const;
    bool operator< (const Author_to_ISBN & other);
    bool operator== (const Author_to_ISBN & other);
};

bool findAuthor(const std::string & au);
bool updateAI(const Author_to_ISBN & ai);
bool deleteAI(const Author_to_ISBN & ai);
