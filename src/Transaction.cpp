#include "Transaction.hpp"
#include <iomanip>

bool TransManager::file_exists(const string& filename) {
        ifstream f(filename);
        return f.good();
    }

TransManager::TransManager(const string& filename) : TransMem(filename){
    if (!file_exists(filename)) {
        TransMem.initialise();
        TransMem.write_info(0, 1);
        trans_times_when_open = 0;
        income_when_open = 0;
        outcome_when_open = 0;
    } else {
        TransMem.get_info(trans_times_when_open, 1);
        TransMem.get_info(income_when_open, 2);
        TransMem.get_info(outcome_when_open, 3);
    }
}

TransManager::~TransManager() {
    for (int i = 0; i < trans_in_thisregis.size(); ++i) {
        Transaction tmp = trans_in_thisregis[i];
        TransMem.write(tmp);
    }
}

void TransManager::addRecorg(const Transaction &other) {
    trans_in_thisregis.push_back(other);
}

void TransManager::searchAllRecord() {
    Transaction tmp;
    int start = sizeof(int);
    int income = income_when_open;
    int outcome = outcome_when_open;
    for (int i = 0; i < trans_in_thisregis.size(); ++i) {
        tmp = trans_in_thisregis[i];
        int a = tmp.amount;
        if (a > 0) {
            income += a;
        } else {
            outcome += a;
        }
    }
    std::cout << std::setprecision(2) << "+ " << income << " - " << -outcome << "\n";
}

bool TransManager::seatchRecord(const int count) {
    Transaction tmp;
    int trans_times_thisregis = trans_in_thisregis.size();
    //回溯的次数
    int back = count - trans_times_thisregis;
    if (back > trans_times_when_open) {
        return false;
    }
    int start = sizeof(int) + (trans_times_when_open - back) * sizeofTr;
    int income = 0;
    int outcome = 0;
    for (int i = trans_times_when_open - back; i < trans_times_when_open; ++i) {
        TransMem.read(tmp, start);
        int a = tmp.amount;
        if (a > 0) {
            income += a;
        } else {
            outcome += a;
        }
        start += sizeofTr;
    }
    for (int i = 0; i < trans_in_thisregis.size(); ++i) {
        tmp = trans_in_thisregis[i];
        int a = tmp.amount;
        if (a > 0) {
            income += a;
        } else {
            outcome += a;
        }
    }
    std::cout << std::setprecision(2) << "+ " << income << " - " << -outcome << "\n";
    return true;
}