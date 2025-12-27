#include <cctype>
#include <iostream>

#include "Book.hpp"
#include "Parser.hpp"
#include "Transaction.hpp"
#include "User.hpp"

int main() {
    UserManager UM;
    TransManager TM("Transfer.data");
    UM.init_root();
    std::string input = {};
    std::getline(std::cin, input);
    InputResult in = inputHandle(input);
    while (in.wordCount != 0 && (in.words[0] == "quit" || in.words[0] == "exit")) {
        if (in.wordCount == 0) {
            continue;
        } else if (in.words[0] == "su") {
            if (in.wordCount == 2) {
                UM.login(in.words[1], "\0");
            } else if (in.wordCount == 3) {
                UM.login(in.words[1], in.words[2]);
            } else {
                std::cout << "Invalid\n";
            }
        } else if (in.words[0] == "logout") {
            if (in.wordCount != 1 || !UM.logout()) {
                std::cout << "Invalid\n";
            }
        } else if (in.words[0] == "register") {
            if (in.wordCount != 4) {
                std::cout << "Invalid\n";
            } else if (!UM.register_user(in.words[1],in.words[2],1,in.words[3])) {
                std::cout << "Invalid\n";
            }
        } else if (in.words[0] == "passwd") {
            if (in.wordCount == 3 && UM.get_current_privilege() == 7) {
                UM.change_password(in.words[1], "\0", in.words[2]);
            } else if (in.wordCount == 4) {
                UM.change_password(in.words[1], in.words[2], in.words[3]);
            } else {
                std::cout << "Invalid\n";
            }
        } else if (in.words[0] == "useradd") {
            if (in.wordCount != 5 || in.words[3][1] != '\0') {
                std::cout << "Invalid\n";
            } else {
                int p = in.words[3][0] - '0';
                if ((p == 1 || p == 3) && !UM.create_user(in.words[1], in.words[2], p, in.words[4])) {
                    std::cout << "Invalid\n";
                } 
            }
        } else if (in.words[0] == "delete") {
            if (in.wordCount != 2) {
                std::cout << "Invalid\n";
            } else if (!UM.delete_user(in.words[1])) {
                std::cout << "Invalid\n";
            }
        } else if (in.words[0] == "show" && in.words[1] != "finance") {
            
        } else if (in.words[0] == "buy") {
            
        } else if (in.words[0] == "select") {
            
        } else if (in.words[0] == "modify") {
            
        } else if (in.words[0] == "import") {
            
        } else if (in.words[0] == "show" && in.words[1] == "finance") {
            
        } else if (in.words[0] == "log") {
            
        } else if (in.words[0] == "report" && in.words[1] == "finance") {
            
        } else if (in.words[0] == "report" && in.words[1] == "employee") {
            
        } else {
            std::cout << "Invalid\n";
        }
        input = {};
        std::getline(std::cin, input);
        InputResult in = inputHandle(input);
    }
    return 0;
}