#pragma once

#include <string>
#include <cctype>  

struct InputResult {
    int wordCount = 0;          // 词数
    std::string words[10] = {};  // 存储每个词
};

//读取命令行整行输入，分词后返回结果结构体
InputResult inputHandle(const std::string input);

// 解析输入字符串，返回类型编号，并将解析出的字符串存到 value 参数中
// 返回值：
//   1: ISBN
//   2: name
//   3: author
//   4: keyword
//   0: 无效格式
int parseInfo_show(const std::string& input, std::string& value);

int parseInfo_modify(const std::string& input, std::string& value);