#pragma once

#include <vector>

#include "Storage.hpp"

struct Transaction {
    bool type;                 // 1"buy" 或 0"import"
    char isbn[MAX_INDEX_LEN];  // 书的编号
    int quantity;              // 交易数量（>0）
    double amount;             // 金额（buy为正，import为负）
    char operator_id[MAX_INDEX_LEN];   // 操作人ID
    Transaction() = default;
};

const int sizeofTr = sizeof(Transaction);

inline std::ostream & operator<< (std::ostream &os, const Transaction& trans) {
    os << trans.amount;
    return os;
}

class TransManager {
    private:
    MemoryRiver <Transaction, 3> TransMem;
    std::vector<Transaction> trans_in_thisregis = {};
    int trans_times_when_open;
    int income_when_open;
    int outcome_when_open;

    public:

    bool file_exists(const string& filename);

    TransManager(const string& filename);

    ~TransManager(); //析构时写入本地文件

    //添加记录
    void addRecorg(const Transaction &);
    //查找全部记录
    void searchAllRecord();
    //部分查找
    bool seatchRecord(const int count);
};