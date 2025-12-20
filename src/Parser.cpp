#include <iostream>
#include <string>
#include <cctype>  

struct InputResult {
    int wordCount;          // 词数
    std::string words[10];  // 存储每个词
};

/**
 * @brief 读取命令行整行输入，分词后返回结果结构体
 * @return InputResult 包含词数和最多10个词的结构体
 * @note 分词规则：按空白字符（空格/制表符/换行等）分隔，连续空白视为一个分隔符，忽略首尾空白
 */
InputResult inputHandle() {
    InputResult result = {0};  // 初始化：词数为0，所有词为空字符串
    std::string input;

    // 读取命令行整行输入（包含空格，直到回车结束）
    std::getline(std::cin, input);
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