#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Book.hpp"
#include "Parser.hpp"
#include "Storage.hpp"
#include "Transaction.hpp"
#include "User.hpp"
#include "Utils.hpp"

int main() {
    UserManager UM;
    TransManager TM("Transfer.data");
    UM.init_root();
    InputResult in;
    do {
        std::string input;
        if (!std::getline(std::cin, input)) {
            return 0;
        }
        in = inputHandle(input);
    } while (in.wordCount == 0);
    while (!(in.wordCount == 1 && (in.words[0] == "quit" || in.words[0] == "exit"))) {
        if (in.words[0] == "su") {
            if (in.wordCount == 2) {
                if (!checkUserID_or_Password(in.words[1]) 
                        || !UM.login(in.words[1], "")) {
                    std::cout << "Invalid\n";
                }
            } else if (in.wordCount == 3) {
                if (!checkUserID_or_Password(in.words[1]) 
                        || !checkUserID_or_Password(in.words[2]) 
                        || !UM.login(in.words[1], in.words[2])) {
                    std::cout << "Invalid\n";
                }
            } else {
                std::cout << "Invalid\n";
            }

        } else if (in.words[0] == "logout") {
            if (in.wordCount != 1 || UM.get_current_privilege() == 0 
                    || !UM.logout()) {
                std::cout << "Invalid\n";
            }

        } else if (in.words[0] == "register") {
            if (in.wordCount != 4 || !checkUserID_or_Password(in.words[1]) 
                    || !checkUserID_or_Password(in.words[2])
                    || !checkUsername(in.words[3])) {
                std::cout << "Invalid\n";
            } else if (!UM.register_user(in.words[1],in.words[2],1,in.words[3])) {
                std::cout << "Invalid\n";
            }

        } else if (in.words[0] == "passwd") {
            if (in.wordCount == 3 && UM.get_current_privilege() == 7) {
                if (!checkUserID_or_Password(in.words[1])
                        || !checkUserID_or_Password(in.words[2])
                        || !UM.change_password(in.words[1], "", in.words[2])) {
                    std::cout << "Invalid\n";
                }
            } else if (in.wordCount == 4) {
                if (UM.get_current_privilege() == 0 || !checkUserID_or_Password(in.words[1])
                        || !checkUserID_or_Password(in.words[2])
                        || !checkUserID_or_Password(in.words[3])
                        || !UM.change_password(in.words[1], in.words[2], in.words[3])) {
                    std::cout << "Invalid\n";      
                }
            } else {
                std::cout << "Invalid\n";
            }

        } else if (in.words[0] == "useradd") {
            if (in.wordCount != 5 || UM.get_current_privilege() < 3
                    || !checkUserID_or_Password(in.words[1])
                    || !checkUserID_or_Password(in.words[2])
                    || !checkPrivilege(in.words[3])
                    || !checkUsername(in.words[4])) {
                std::cout << "Invalid\n";
            } else if (!UM.create_user(in.words[1], in.words[2], in.words[3][0] - '0', in.words[4])) {
                    std::cout << "Invalid\n";
                } 

        } else if (in.words[0] == "delete") {
            if (in.wordCount != 2 || UM.get_current_privilege() != 7
                    || !checkUserID_or_Password(in.words[1])) {
                std::cout << "Invalid\n";
            } else if (!UM.delete_user(in.words[1])) {
                std::cout << "Invalid\n";
            }

        } else if ( (in.wordCount == 1 && in.words[0] == "show")
                 || in.words[0] == "show" && in.wordCount ==2 && in.words[1] != "finance") {
            if (UM.get_current_privilege() == 0) {
                std::cout << "Invalid\n";
            } else {
                if (in.wordCount == 1) {
                    if(!show_all()) {
                        std::cout << "\n";
                    }
                } else {
                    std::string value = {};
                    int type = parseInfo_show(in.words[1], value);
                    if (type == 0) {
                        std::cout << "Invalid\n";
                    } else if (type == 1) { //isbn
                        if (!print_book(value)) {
                            std::cout << "\n";
                        }
                    } else if (type == 2) { //name
                        std::vector<std::string> ISBNs;
                        ISBNs.clear();
                        findName(value,ISBNs);
                        std::sort(ISBNs.begin(), ISBNs.end());
                        auto last = std::unique(ISBNs.begin(), ISBNs.end());
                        ISBNs.erase(last, ISBNs.end());
                        bool empty = true;
                        for (size_t i = 0; i < ISBNs.size(); ++i) {
                            if (print_book(ISBNs[i])) {
                                empty = false;
                            }
                        }
                        if (empty) {
                            std::cout << "\n";
                        }
                    } else if (type == 3) { //author
                        std::vector<std::string> ISBNs;
                        ISBNs.clear();
                        findAuthor(value, ISBNs);
                        std::sort(ISBNs.begin(), ISBNs.end());
                        auto last = std::unique(ISBNs.begin(), ISBNs.end());
                        ISBNs.erase(last, ISBNs.end());
                        bool empty = true;
                        for (size_t i = 0; i < ISBNs.size(); ++i) {
                            empty = false;
                            print_book(ISBNs[i]);
                        }
                        if (empty) {
                            std::cout << "\n";
                        }
                    } else if (type == 4) { //keyword
                        std::vector<std::string> ISBNs;
                        ISBNs.clear();
                        findkeyW(value, ISBNs);
                        std::sort(ISBNs.begin(), ISBNs.end());
                        auto last = std::unique(ISBNs.begin(), ISBNs.end());
                        ISBNs.erase(last, ISBNs.end());
                        bool empty = true;
                        for (size_t i = 0; i < ISBNs.size(); ++i) {
                            empty = false;
                            print_book(ISBNs[i]);
                        }
                        if (empty) {
                            std::cout << "\n";
                        }
                    }
                }
            }
            
        } else if (in.words[0] == "buy") {
            Book bk = {};
            int num = 0;
            if (in.wordCount != 3 || UM.get_current_privilege() == 0
                    || !checkISBN(in.words[1]) || !checkQuantity(in.words[2], num)) {
                std::cout << "Invalid\n";
            } else {
                if (load_book(in.words[1], bk)) {  
                    Book newbk = bk;
                    if (newbk.decrease_quantity(num)) {
                        UM.update_book_stack(bk, newbk);
                        Transaction ta;
                        ta.quantity = num;
                        ta.type = true;
                        strcpy(ta.isbn, newbk.isbn);
                        std::strncpy(ta.operator_id, UM.get_current_user_id().c_str(), MAX_INDEX_LEN - 1);
                        ta.operator_id[MAX_INDEX_LEN - 1] = '\0';
                        ta.amount = (bk.price_up * UP + bk.price_down) * num;
                        TM.addRecorg(ta);
                        long long total_price = (bk.price_up * UP + bk.price_down) * num;
                        long long price_int = total_price / 100;
                        int price_dec = total_price % 100;
                        std::cout << price_int << "."
                            << std::setw(2) << std::setfill('0') << price_dec << "\n";
                    } else {
                        std::cout << "Invalid\n";
                    }       
                } else {
                    std::cout << "Invalid\n";
                }
            }

        } else if (in.words[0] == "select") {
            Book bk;
            if (in.wordCount != 2 || !checkISBN(in.words[1]) || UM.get_current_privilege() < 3) {
                std::cout << "Invalid\n";
            } else if (load_book(in.words[1], bk)) {
                UM.select_book(bk);
            } else {
                Book new_book(in.words[1]); //新建书
                update_book(new_book);
                UM.select_book(new_book);
            }

        } else if (in.words[0] == "modify") {
            if (UM.get_current_privilege() < 3 || UM.has_selected.empty() || !UM.has_selected.back()) {
                std::cout << "Invalid\n";
            } else {
                int times = in.wordCount - 1;
                if (times > 5) {
                    std::cout << "Invalid\n";
                } else {
                    std::string values[5] = {};
                    std::vector<int> type;
                    bool flag = true;
                    for (int i = 1; i <= times; ++i) {
                        int tmp = parseInfo_modify(in.words[i], values[i - 1]);
                        if (tmp == 0) {
                            flag = false;
                            break;
                        }
                        for (size_t j = 0; j < type.size(); ++j) {
                            if (tmp == type[j]) {
                                flag = false;
                                break;
                            }
                        }
                        if (flag) {
                            type.push_back(tmp);
                        } else break;
                    }
                    if (!flag) {
                        std::cout << "Invalid\n";
                    } else {
                        Book bk;
                        UM.get_select_book(bk);

                        bool ok = true;
                        int isbn_idx = -1;

                        // 先应用非 ISBN 的修改
                        for (int i = 0; i < times; ++i) {
                            if (type[i] == 1) { isbn_idx = i; continue; }
                            if (type[i] == 2) {
                                if (!bk.set_name(values[i])) { ok = false; break; }
                            } else if (type[i] == 3) {
                                if (!bk.set_author(values[i])) { ok = false; break; }
                            } else if (type[i] == 4) {
                                if (!bk.set_keyword(values[i])) { ok = false; break; }
                            } else if (type[i] == 5) {
                                long long p = 0;
                                if (!checkPrice_or_TotalCost(values[i], p) || !bk.set_price(p)) {
                                    ok = false; break;
                                }
                            }
                        }

                        if (!ok) {
                            std::cout << "Invalid\n";
                        } else {
                            // 最后处理 ISBN（如果有）
                            if (isbn_idx != -1) {
                                if (!bk.set_isbn(values[isbn_idx])) {
                                    std::cout << "Invalid\n";
                                    ok = false;
                                }
                            }
                            // 若全部成功，刷新选中书以同步 UM 状态与存储
                            if (ok) {
                                UM.select_book(bk);
                            }
                        }
                    }
                }
            }

        } else if (in.words[0] == "import") {
            Book bk;
            int quan;
            long long num;
            if (in.wordCount != 3 || UM.get_current_privilege() < 3 || UM.has_selected.empty() || !UM.has_selected.back()
                    || !checkQuantity(in.words[1], quan) || !checkPrice_or_TotalCost(in.words[2], num)) {
                std::cout << "Invalid\n";
            } else {
                if (UM.get_select_book(bk)) {
                    Book newbk = bk;
                    if (newbk.increase_quantity(quan)) {
                        UM.update_book_stack(bk, newbk);
                        Transaction ta;
                        ta.quantity = quan;
                        ta.type = false;
                        strcpy(ta.isbn, bk.isbn);
                        std::strncpy(ta.operator_id, UM.get_current_user_id().c_str(), MAX_INDEX_LEN - 1);
                        ta.operator_id[MAX_INDEX_LEN - 1] = '\0';
                        ta.amount = -num;
                        TM.addRecorg(ta);
                    } else {
                        std::cout << "Invalid\n";
                    }
                } else {
                    std::cout << "Invalid\n";
                }
            }
        } else if ((in.words[0] == "show" && in.wordCount > 1 && in.words[1] == "finance")) {
            int num;
            if (UM.get_current_privilege() != 7) {
                std::cout << "Invalid\n";
            } else if (in.wordCount == 2) {
                TM.searchAllRecord();
            } else if (in.wordCount == 3 && checkQuantity(in.words[2], num)) {
                if (!TM.seatchRecord(num)) {
                    std::cout << "Invalid\n";
                }
            } else {
                std::cout << "Invalid\n";
            }
        } else if (in.words[0] == "log") {
            //todo
        } else if (in.words[0] == "report" && in.wordCount > 1 && in.words[1] == "finance") {
            //todo
        } else if (in.words[0] == "report" && in.wordCount > 1 && in.words[1] == "employee") {
            //todo
        } else {
            std::cout << "Invalid\n";
        }
        do {
            std::string input;
            if (!std::getline(std::cin, input)) {
                return 0;
            }
            in = inputHandle(input);
        } while (in.wordCount == 0);
    }
    return 0;
}