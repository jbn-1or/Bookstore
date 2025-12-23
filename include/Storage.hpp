#pragma once

// 提供所有文件IO操作和读取的接口
#include <iostream>
#include <cstring>
#include <string>
#include <climits>
#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out | std::ios::binary);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        // 以二进制读模式打开文件
        file.open(file_name, std::ios::in | std::ios::binary);
        if (!file.is_open()) return;
        // 计算第n个int的偏移量（1-base转0-base）
        std::streampos offset = (n - 1) * sizeof(int);
        // 移动读指针到目标位置
        file.seekg(offset);
        // 读取int值到tmp
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        // 以二进制读写模式打开文件
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        // 计算第n个int的偏移量
        std::streampos offset = (n - 1) * sizeof(int);
        // 移动写指针到目标位置
        file.seekp(offset);
        // 写入tmp到指定位置
        file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        /* your code here */
        // 以二进制读写模式打开文件
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return -1;
        // 移动写指针到文件末尾（新对象写入末尾）
        file.seekp(0, std::ios::end);
        // 获取当前位置作为写入索引（起始偏移量）
        int index = static_cast<int>(file.tellp());
        // 写入T类型对象
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
        return index;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        // 以二进制读写模式打开文件
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) return;
        // 移动写指针到指定索引位置
        file.seekp(index);
        // 写入新的对象值覆盖原有内容
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        // 以二进制读模式打开文件
        file.open(file_name, std::ios::in | std::ios::binary);
        if (!file.is_open()) return;
        // 移动读指针到指定索引位置
        file.seekg(index);
        // 读取对象值到t
        file.read(reinterpret_cast<char*>(&t), sizeofT);
        file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        // 不涉及空间回收，空实现
    }
};

template <class Block_T>
class BlockManager{
    private:
    std::string filename;
    MemoryRiver<Block_T> mr;
    public:
    BlockManager(std::string name) : filename(name), mr(name) {
        // 初始化文件（仅首次创建时生效，若文件已存在则跳过）
        std::ifstream check(name);
        if (!check.is_open()) {
            mr.initialise(name);
        }
        check.close();
    }
    
    Block_T find(int type, const char * x);
    void insert(const Block_T x);
    void delete_(const char * x);
};

struct UserBlock {
    char user_id[100];       // 登录ID（唯一）
    char password[100];      // 密码
    int privilege;           // 权限等级（1/3/7）
    char username[100];      // 用户名
};

struct BookBlock {
    char isbn[100];          // 唯一标识
    char name[100];          // 书名
    char author[100];        // 作者
    char keyword[100];       // 关键词（|分隔，无重复）
    double price;            // 单价（保留2位小数）
    int quantity;            // 库存数量（≥0）
};

struct LogBlock {
};

BlockManager<UserBlock> userDataMG("User.data");
BlockManager<BookBlock> bookDataMG("Book.data");
BlockManager<LogBlock> logDataMG("Log.data");

