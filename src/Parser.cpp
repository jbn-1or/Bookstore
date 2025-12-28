#include "Parser.hpp"
#include "Utils.hpp"

InputResult inputHandle(const std::string input) {
    InputResult result;
    // 读取命令行整行输入（包含空格，直到回车结束）
    int inputLen = input.length();
    int i = 0;
    // 跳过行首的空白字符（空格、制表符\t、换行\n等）
    while (i < inputLen && std::isspace(static_cast<unsigned char>(input[i]))) {
        ++i;
    }
    // 分词
    while (i < inputLen && result.wordCount < 10) {
        std::string currentWord;  // 存储当前正在解析的词
        // 提取当前词的所有字符（直到遇到空白字符）
        while (i < inputLen && !std::isspace(static_cast<unsigned char>(input[i]))) {
            currentWord += input[i];
            ++i;
        }
        // 将当前词存入结果（确保非空）
        if (!currentWord.empty()) {
            result.words[result.wordCount] = currentWord;
            result.wordCount++;
        }
        // 跳过当前词后的空白字符
        while (i < inputLen && std::isspace(static_cast<unsigned char>(input[i]))) {
            ++i;
        }
    }
    return result;
}

int parseBookInfo(const std::string& input, std::string& value) {
    value.clear(); // 清空旧数据
    if (input.substr(0, 6) == "-ISBN=") {
        value = input.substr(6);
        if (checkISBN(value)) {
            return 1;
        }
        return 0;
    }
    else if (input.substr(0, 7) == "-name=\"") {
        size_t endQuote = input.find_last_of('"');
        if (endQuote != std::string::npos && endQuote > 7) {
            value = input.substr(7, endQuote - 7);
            if (checkBookName_or_Author(value)) {
                return 2;
            } else {
                return 0;
            }
        }
    }
    else if (input.substr(0, 9) == "-author=\"") {
        size_t endQuote = input.find_last_of('"');
        if (endQuote != std::string::npos && endQuote > 9) {
            value = input.substr(9, endQuote - 9);
            if (checkBookName_or_Author(value)) {
                return 3;
            } else {
                return 0;
            }
        }
    }
    else if (input.substr(0, 9) == "-keyword=\"") {
        size_t endQuote = input.find_last_of('"');
        if (endQuote != std::string::npos && endQuote > 9) {
            value = input.substr(9, endQuote - 9);
            if (value.size() > 60) {
                return 0;
            }
            for (char c : value) {
                char ch = static_cast<char>(c);
                if (!((ch >= 33 && ch <= 126) && (ch != '"') && (ch != '|'))) {
                    return 0;
                }
            }
            return 4;
        }
    }
    return 0; // 不匹配任何格式
}