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
#include <cstdlib>
#include <ctime>
using namespace std;

class RandomCache
{
private:
    long long int capacity;
    unordered_map<long long int, long long int> cache; //id->size
    vector<long long int> vec;                         //按加入缓存顺序将key排序
    long long int cache_size = 0, hit_num = 0, total_num = 0;

public:
    RandomCache(long long int _capacity) : capacity(_capacity)
    {
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
                if (value > capacity)
                    return;
                evict(key, value);
                set(key, value);
            }
        }
    }

    long long int getTotal() const
    {
        return total_num;
    }

    long long int getHit() const
    {
        return hit_num;
    }

private:
    int my_rand(int end)
    {
        return rand() % end;
    }

    bool get(long long int key) const
    {
        return cache.count(key);
    }
    void set(long long int key, long long int value)
    {
        vec.push_back(key);
        cache[key] = value;
        cache_size += value;
    }
    void evict(long long int key, long long int value)
    {
        while (cache_size + value > capacity)
        {
            int idx = my_rand(vec.size());
            long long int evict_key = vec[idx];
            swap(vec[idx], vec.back());
            vec.pop_back();
            cache_size -= cache[evict_key];
            cache.erase(evict_key);
        }
    }
};

int main()
{
    srand((unsigned)time(NULL));
    string line, temp;
    stringstream splitline;

    //交互
    long long int capa;
    cout << "Enter RandomCache Capacity (Bytes) : ";
    cin >> capa;
    string test;
    cout << "Enter <Your Test File>: ";
    cin >> test;

    fstream fin_test(test);
    RandomCache cache(capa);

    while (getline(fin_test, line))
    {
        splitline.clear();
        splitline.str(line);
        vector<string> data;
        while (getline(splitline, temp, ' '))
        {
            data.push_back(temp);
        }
        //检查是否有某个块比缓存容量还大
        long long int key = stoll(data[1]), value = stoll(data[0]);

        cache.visit(key, value);
    }
    cout << "-------Random替换算法-------" << endl;
    cout << "缓存容量 (Bytes) ：" << capa << endl;
    cout << "总请求数: " << cache.getTotal() << ", 命中次数: " << cache.getHit() << endl;
    cout << "-------Random替换算法-------" << endl;

    fin_test.close();
}