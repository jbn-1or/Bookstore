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
        std::string input = {};
        std::getline(std::cin, input);
        in = inputHandle(input);
    } while (in.wordCount == 0);
    while (in.wordCount == 1 && (in.words[0] == "quit" || in.words[0] == "exit")) {
        if (in.words[0] == "su") {
            if (in.wordCount == 2) {
                if (!checkUserID_or_Password(in.words[1]) 
                        || !UM.login(in.words[1], "\0")) {
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
                    || !checkUserID_or_Password(in.words[1]) || !UM.logout()) {
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
                        || !UM.change_password(in.words[1], "\0", in.words[2])) {
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
                    || ! checkUserID_or_Password(in.words[1])
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

        } else if (in.words[0] == "show" && in.words[1] != "finance") {
            std::string value = {};
            int type = parseBookInfo(in.words[1], value);
            if (type == 0) {
                std::cout << "Invalid\n";
            } else if (type == 1) { //isbn
                print_book(value);
            } else if (type == 2) { //name
                std::vector<std::string> ISBNs = {};
                findName(value,ISBNs);
                for (int i = 0; i < ISBNs.size(); ++i) {
                    print_book(ISBNs[i]);
                }
            } else if (type == 3) { //auther
                std::vector<std::string> ISBNs = {};
                findAuthor(value, ISBNs);
                for (int i = 0; i < ISBNs.size(); ++i) {
                    print_book(ISBNs[i]);
                }
            } else if (type == 4) { //keword
                std::vector<std::string> ISBNs = {};
                findkeyW(value, ISBNs);
                for (int i = 0; i < ISBNs.size(); ++i) {
                    print_book(ISBNs[i]);
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
                    if (newbk.increase_quantity(num)) {
                        UM.update_book_stack(bk,newbk);
                        Transaction ta;
                        ta.quantity = num;
                        ta.type = true;
                        strcpy(ta.isbn, newbk.isbn);
                        std::strncpy(ta.operator_id , UM.get_current_user_id().c_str(), MAX_INDEX_LEN - 1);
                        ta.operator_id[MAX_INDEX_LEN - 1] = '\0';
                        ta.amount = (bk.price_up * UP + bk.price_down) * num;
                        TM.addRecorg(ta);
                    } else {
                        std::cout << "Invalid\n";
                    }
                } else {
                    std::cout << "Invalid\n";
                }
            }

        } else if (in.words[0] == "select") {
            Book bk;
            if (in.wordCount != 2 || !checkISBN(in.words[1])) {
                std::cout << "Invalid\n";
            } else if (load_book(in.words[1], bk)) {
                UM.select_book(bk);
            } else {
                Book(in.words[1]); //新建书
            }

        } else if (in.words[0] == "modify") {
            if (UM.get_current_privilege() < 3 || !UM.has_selected.back()) {
                std::cout << "Invalid\n";
            }
            int times = in.wordCount - 1;
            if (times > 5) {
                std::cout << "Invalid\n";
            }
            std::string values[5];
            std::vector<int> type;
            bool flag = true;
            for (int i = 0; i < times; ++i) {
                int tmp = parseBookInfo(in.words[1], values[1]);
                for (int j = 0; j < type.size(); ++j) {
                    if (tmp == type[j] || tmp == 0) {
                        flag = false;
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
                for (int i = 0; i < times; ++i) {
                    if (type[i] == 1) {
                        bk.set_isbn(values[i]);
                    } else if (type[i] == 2) {
                        bk.set_name(values[i]);
                    } else if (type[i] == 3) {
                        bk.set_author(values[i]);
                    } else if (type[i] == 4) {
                        bk.set_keyword(values[i]);
                    } 
                }
            }

        } else if (in.words[0] == "import") {
            Book bk;
            int quan;
            long long num;
            if (in.wordCount != 3 || UM.get_current_privilege() < 3 || !UM.has_selected.back()
                    || !checkQuantity(in.words[1], quan) || !checkPrice_or_TotalCost(in.words[2], num)) {
                std::cout << "Invalid\n";
            } else {
                if (load_book(in.words[1], bk)) {
                    Book newbk = bk;
                    if (newbk.increase_quantity(num)) {
                        UM.update_book_stack(bk,newbk);
                        Transaction ta;
                        ta.quantity = num;
                        ta.type = false;
                        strcpy(ta.isbn, bk.isbn);
                        std::strncpy(ta.operator_id , UM.get_current_user_id().c_str(), MAX_INDEX_LEN - 1);
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
        } else if (in.words[0] == "show" && in.words[1] == "finance") {
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
        } else if (in.words[0] == "report" && in.words[1] == "finance") {
            //todo
        } else if (in.words[0] == "report" && in.words[1] == "employee") {
            //todo
        } else {
            std::cout << "Invalid\n";
        }
        do {
            std::string input = {};
            std::getline(std::cin, input);
            in = inputHandle(input);
        } while (in.wordCount == 0);
    }
    return 0;
}