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
        income_when_open_up = 0; //乘100的前
        income_when_open_down = 0; //乘100的后8位
        outcome_when_open_up = 0;
        outcome_when_open_down = 0;
    } else {
        TransMem.get_info(trans_times_when_open, 1);
        TransMem.get_info(income_when_open_up, 2);
        TransMem.get_info(income_when_open_down, 3);
        TransMem.get_info(outcome_when_open_up, 4);
        TransMem.get_info(outcome_when_open_down, 5);
    }
}

TransManager::~TransManager() {
    long long income = income_when_open_down + UP * income_when_open_up;
    long long outcome = outcome_when_open_down + UP * outcome_when_open_up;
    for (int i = 0; i < trans_in_thisregis.size(); ++i) {
        Transaction tmp = trans_in_thisregis[i];
        TransMem.write(tmp);
        tmp = trans_in_thisregis[i];
        long long a = tmp.amount;
        if (a > 0) {
            income += a;
        } else {
            outcome += a;
        }
    }
    int income_up = income / UP;
    int income_down = income % UP;
    long long abs_ot = -outcome;
    int outcome_up = abs_ot / UP;
    int outcome_down = abs_ot % UP;
    TransMem.write_info(trans_times_when_open + trans_in_thisregis.size(), 1);
    TransMem.write_info(income_up, 2);
    TransMem.write_info(income_down, 3);
    TransMem.write_info(outcome_up, 4);
    TransMem.write_info(outcome_down, 5);
}

void TransManager::addRecorg(const Transaction &other) {
    trans_in_thisregis.push_back(other);
}

void TransManager::searchAllRecord() {
    Transaction tmp;
    long long income = income_when_open_down + UP * income_when_open_up;
    long long outcome = outcome_when_open_down + UP * outcome_when_open_up;
    for (int i = 0; i < trans_in_thisregis.size(); ++i) {
        tmp = trans_in_thisregis[i];
        long long a = tmp.amount;
        if (a > 0) {
            income += a;
        } else {
            outcome += a;
        }
    }
    long long income_int = income / 100;
    int income_dec = income % 100;
    long long abs_ot = -outcome;
    long long outcome_int = abs_ot / 100;
    int outcome_dec = abs_ot % 100;
    std::cout << "+ " << income_int << "." 
        << std::setw(2) << std::setfill('0') << income_dec;
    std::cout << " - " << outcome_int << "."
        << std::setw(2) << std::setfill('0')  << outcome_dec << "\n";
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
    long long income = 0;
    long long outcome = 0;
    for (int i = trans_times_when_open - back; i < trans_times_when_open; ++i) {
        TransMem.read(tmp, start);
        long long a = tmp.amount;
        if (a > 0) {
            income += a;
        } else {
            outcome += a;
        }
        start += sizeofTr;
    }
    for (int i = 0; i < trans_in_thisregis.size(); ++i) {
        tmp = trans_in_thisregis[i];
        long long a = tmp.amount;
        if (a > 0) {
            income += a;
        } else {
            outcome += a;
        }
    }
    long long income_int = income / 100;
    int income_dec = income % 100;
    long long abs_ot = -outcome;
    long long outcome_int = abs_ot / 100;
    int outcome_dec = abs_ot % 100;
    std::cout << "+ " << income_int << "." 
        << std::setw(2) << std::setfill('0') << income_dec;
    std::cout << " - " << outcome_int << "."
        << std::setw(2) << std::setfill('0')  << outcome_dec << "\n";
    return true;
}