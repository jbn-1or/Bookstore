#pragma once

// 提供所有文件IO操作和读取的接口
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <ostream>
#include <string>
#include <climits>
#include <cassert>
#include <algorithm>
#include <vector>

const int MAX_ENTRIES_PER_BLOCK = 5000;
const int MAX_INDEX_LEN = 64 + 1;

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 1>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    const int sizeofT = sizeof(T);
    bool is_open_ = false;

    // 确保文件打开
    void ensure_open() {
        if (!is_open_) {
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            if (!file.is_open()) {
                // 若文件不存在（仅initialise时），先以out模式创建
                file.open(file_name, std::ios::out);
                file.close();
                file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            }
            is_open_ = file.is_open();
        }
    }

public:
    MemoryRiver() = default;
    MemoryRiver(const string& file_name) : file_name(file_name) {}

    // 析构函数：关闭文件
    ~MemoryRiver() {
        if (is_open_) {
            file.close();
            is_open_ = false;
        }
    }

    // 初始化文件：写入info_len个初始值为0的int
    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out | std::ios::trunc);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
        is_open_ = false;
    }

    // 读取第n个int元信息（1-base）
    void get_info(int& tmp, int n) {
        if (n < 1 || n > info_len) return;
        ensure_open();
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
    }

    // 写入第n个int元信息（1-base）
    void write_info(int tmp, int n) {
        if (n < 1 || n > info_len) return;
        ensure_open();
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
    }

    // 写入T类型对象，返回写入的起始位置
    int write(T& t) {
        ensure_open();
        file.seekp(0, std::ios::end);
        int index = static_cast<int>(file.tellp());
        file.write(reinterpret_cast<char*>(&t), sizeofT);
        return index;
    }

    // 更新指定位置的T类型对象
    void update(T& t, const int index) {
        if (index < 0) return;
        ensure_open();
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    // 读取指定位置的T类型对象
    void read(T& t, const int index) {
        if (index < 0) return;
        ensure_open();
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&t), sizeofT);
    }

    //void Delete(int index) {}
};

// 块链表模板
template<class Tem>
struct BlockList {
    int next_block_id;                    // 下一个块的存储位置（-1表示无）
    int num_entries;                      // 本块已存储的条目数
    Tem ob[MAX_ENTRIES_PER_BLOCK];        // 块内存储的条目数组
};

// 块链表管理模板类
template<class Tem>
class BlockListManager {
private:
    MemoryRiver<BlockList<Tem>, 4> mem_river;  // 文件管理对象
    int first_block_id_ = -1;     // 第一个块的存储位置
    int max_idx_len_ = MAX_INDEX_LEN;             // 索引最大长度
    int max_entries_per_block_ = MAX_ENTRIES_PER_BLOCK;   // 每个块最大条目数
public:
    // 类型别名
    using BlockType = BlockList<Tem>;
    using RiverType = MemoryRiver<BlockType, 4>;

    bool file_exists(const string& filename) {
        ifstream f(filename);
        return f.good();
    }
    // 构造函数

    BlockListManager(const string& filename, int max_idx_len, int max_entries_per_block)
        : mem_river(filename), max_idx_len_(max_idx_len), max_entries_per_block_(max_entries_per_block) {
        if (!file_exists(filename)) {
            // 文件不存在：初始化 + 创建第一个块
            mem_river.initialise();
            mem_river.write_info(max_idx_len_, 1);
            mem_river.write_info(max_entries_per_block_, 2);
            mem_river.write_info(sizeof(BlockType), 3);

            BlockType first_block;
            first_block.next_block_id = -1;
            first_block.num_entries = 0;
            first_block_id_ = mem_river.write(first_block);
            mem_river.write_info(first_block_id_, 4);
        } else {
            // 文件存在：读取元信息和第一个块位置
            int read_idx_len, read_entries, read_block_size;
            mem_river.get_info(read_idx_len, 1);
            mem_river.get_info(read_entries, 2);
            mem_river.get_info(read_block_size, 3);
            max_idx_len_ = read_idx_len;
            max_entries_per_block_ = read_entries;
            mem_river.get_info(first_block_id_, 4);
        }
    }

    // 插入条目（块满则新建块）
    void insertEntry(const Tem& entry) {
        int current_block_id = first_block_id_;
        BlockType current_block{};
        int insert_position = -1;
        while (true) {
            //读取当前块
            mem_river.read(current_block, current_block_id);
            //若当前块无内容
            if (current_block.num_entries == 0) {
                current_block.num_entries = 1;
                current_block.ob[0] = entry;
                mem_river.update(current_block, current_block_id);
                return;
            } else if (entry < current_block.ob[0]) { // 比第一条还小（仅可能在当前块是第一块时发生）
                //在这一块头部插入块
                insert_position = 0;
                break;
            } else if (current_block.ob[current_block.num_entries - 1] < entry) { //比最后一条还大
                if (current_block.next_block_id == -1) { //在这一块末尾插入
                    insert_position = current_block.num_entries;
                    break;
                }
                BlockType next_block{};
                mem_river.read(next_block, current_block.next_block_id);
                if (entry < next_block.ob[0]) { //在这一块末尾插入
                    insert_position = current_block.num_entries;
                    break;
                }// 否则去下一块
            } else  { // 一定在本块内
                //找到插入的地方
                int it = std::upper_bound(current_block.ob,current_block.ob + current_block.num_entries, entry) - current_block.ob;
                //在这个地方插入
                insert_position = it;
                break;
            }
            current_block_id = current_block.next_block_id;
        }
        //在当前块插入条目
        //临时储存
        Tem tmp[MAX_ENTRIES_PER_BLOCK + 1];
        for (int i = 0; i < insert_position; ++i) {
            tmp[i] = current_block.ob[i];
        }
        tmp[insert_position] = entry;
        for (int i = insert_position + 1; i <= current_block.num_entries; ++i) {
            tmp[i] = current_block.ob[i - 1];
        }
        //讨论是否分块
        if (current_block.num_entries == max_entries_per_block_) { //分块
            int old_next_block_id = current_block.next_block_id;  //之前下一块的位置
            int new_num_entry = (max_entries_per_block_ + 1)/ 2;        //分块后这一块的条目数
            int new_next_num_entry = (max_entries_per_block_ + 1) - new_num_entry; //分块后新块的条目数
            //更新两块的条目
            for (int i = 0; i < new_num_entry; ++i) {
                current_block.ob[i] = tmp[i];
            }
            BlockType new_next_block{};
            for (int i = new_num_entry; i < max_entries_per_block_ + 1; ++i) {
                new_next_block.ob[i - new_num_entry] = tmp[i];
            }
            //更新两块的成员
            current_block.num_entries = new_num_entry;
            new_next_block.num_entries = new_next_num_entry;
            new_next_block.next_block_id = old_next_block_id;
            int new_next_block_id = mem_river.write(new_next_block); //写入新块
            current_block.next_block_id = new_next_block_id;
            mem_river.update(current_block, current_block_id); //更新本块
        } else { //不用分块
        current_block.num_entries++;
        for (int i = 0; i < current_block.num_entries; ++i) {
            current_block.ob[i] = tmp[i];
        }
        // 写回更新后的块
        mem_river.update(current_block, current_block_id);
        }
    }

    // 删除条目
    void deleteEntry(const Tem& entry) {
        int current_block_id = first_block_id_;
        BlockType current_block{};
        while (current_block_id != -1) {
            mem_river.read(current_block, current_block_id);
            if (current_block.ob[current_block.num_entries - 1] < entry) {
                current_block_id = current_block.next_block_id;
            } else if (entry < current_block.ob[0]) {
                break;
            } else{
                int it = std::lower_bound(current_block.ob,current_block.ob + current_block.num_entries, entry) - current_block.ob;
                if (entry == current_block.ob[it]) {
                    for (int i = it; i < current_block.num_entries - 1; i++) {
                        current_block.ob[i] = current_block.ob[i + 1];
                    }
                    current_block.ob[current_block.num_entries - 1] = Tem();
                    current_block.num_entries--;
                    mem_river.update(current_block, current_block_id);
                }
                else break;
            }
            // 跳转到下一个块
            current_block_id = current_block.next_block_id;
        }
    }

    // 查找条目
    bool findEntry(const string& index, std::vector<Tem>& arr) {
        int current_block_id = first_block_id_;
        BlockType current_block{};
        bool unfind = true;
        while (current_block_id != -1) {
            mem_river.read(current_block, current_block_id);
            if (current_block.ob[current_block.num_entries - 1].get_index() == index
                && current_block.ob[0].get_index() == index) {  //整组都是，全加上
                    unfind = false;
                    for (int i = 0; i < current_block.num_entries; ++i) {
                        arr.push_back(current_block.ob[i]);
                    }
            } else if (current_block.ob[current_block.num_entries - 1].get_index() < index) { //在最后面一个之后
                current_block_id = current_block.next_block_id;
            } else if (index < current_block.ob[0].get_index() && unfind) { //在最前面一个之前且前面还没有
                return false;
            } else if (index < current_block.ob[0].get_index() && !unfind) {
                break;
            } else {  //在这块的范围内
                //找到最靠前的等于index的
                int first_id = 0, last_id = current_block.num_entries;
                int low = 0, high = current_block.num_entries - 1;
                int mid = 0;
                while (high > low) {
                    mid = (low + high) / 2;
                    if (current_block.ob[mid].get_index() < index) {
                        low = mid + 1;
                    } else {
                        high = mid;
                    }
                }
                first_id = low;
                //找到最靠后的后等于的index
                low = 0;
                high = current_block.num_entries - 1;
                while (high > low) {
                    int mid = (low + high + 1) / 2;
                    if (current_block.ob[mid].get_index() > index) {
                        high = mid - 1;
                    } else {
                        low = mid;
                    }
                }
                last_id = low;
                //检查得到的上下限是否是index
                if (current_block.ob[first_id].get_index() == index) {
                    unfind = false;
                    for (int i = first_id; i <= last_id; ++i) {
                        Tem tmp = current_block.ob[i];
                        arr.push_back(tmp);
                    }
                } else { //不是说明不存在
                    return false;
                }

                if (last_id < current_block.num_entries - 1) { //都在这一块内了
                    return true;
                }
            }
            // 跳转到下一个块
            current_block_id = current_block.next_block_id;
        }
        if (unfind) {
            return false;
        }
        else return true;
    }

    bool printAllEntries() {
        int current_block_id = first_block_id_;
        BlockType current_block{};
        bool not_empty = false;
        while (current_block_id != -1) {
            mem_river.read(current_block, current_block_id);
            if (current_block.num_entries == 0) {
                continue;
            } else {
                not_empty = true;
                for (int i = 0; i < current_block.num_entries; ++i) {
                    std::cout << current_block.ob[i];
                }
            }
            current_block_id = current_block.next_block_id;
        }
        return not_empty;
    }
};
