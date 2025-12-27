#pragma once

#include <string>
#include <cctype>  

struct InputResult {
    int wordCount = 0;          // 词数
    std::string words[10] = {};  // 存储每个词
};

//读取命令行整行输入，分词后返回结果结构体
InputResult inputHandle(const std::string input);