#include <iostream>

#include "Parser.hpp"

int main() {
    std::string input;
    std::getline(std::cin, input);
    InputResult in = inputHandle(input);
    std::cout << in.wordCount << "\n";
    for (int i = 0; i < in.wordCount; ++i) {
        std::cout << in.words[i] << " ";
    }
    return 0;
}