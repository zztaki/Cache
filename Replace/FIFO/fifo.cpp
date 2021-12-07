#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class FIFOCache
{
public:
    long long int capacity;
    unordered_map<long long int, long long int> cache; //id->size
    queue<long long int> ordered;                      //按加入缓存顺序将key排序
    long long int cache_size, hit_num, total_num;
    FIFOCache(long long int capacity)
    {
        cache_size = hit_num = total_num = 0;
        this->capacity = capacity;
    }
    bool get(long long int key)
    {
        return cache.count(key);
    }
    void set(long long int key, long long int value)
    {
        ordered.push(key);
        cache[key] = value;
        cache_size += value;
    }
    void evict(long long int key, long long int value)
    {
        while (cache_size + value > capacity)
        {
            long long int evict_key = ordered.front();
            ordered.pop();
            cache_size -= cache[evict_key];
            cache.erase(evict_key);
        }
    }
    void visit(long long int key, long long int value)
    {
        total_num++;

        if (get(key))
        {
            hit_num++; // FIFO命中没有任何promotion
        }
        else //miss
        {
            if (cache_size + value <= capacity)
            {
                set(key, value);
            }
            else //淘汰
            {
                evict(key, value);
                set(key, value);
            }
        }
    }
};

int main()
{
    string line, temp;
    stringstream splitline;

    //交互
    long long int capa;
    cout << "Enter FIFOCache Capacity (Bytes) : ";
    cin >> capa;
    string test;
    cout << "Enter <Your Test File>: ";
    cin >> test;

    fstream fin_test(test);
    FIFOCache cache(capa);

    while (getline(fin_test, line))
    {
        splitline.clear();
        splitline.str(line);
        vector<string> data;
        while (getline(splitline, temp, ' '))
        {
            data.push_back(temp);
        }
        cache.visit(stoll(data[1]), stoll(data[0]));
    }
    cout << "-------FIFO替换算法-------" << endl;
    cout << "缓存容量 (Bytes) ：" << capa << endl;
    cout << "总请求数: " << cache.total_num << ", 命中次数: " << cache.hit_num << endl;
    cout << "-------LRU替换算法-------" << endl;
}
