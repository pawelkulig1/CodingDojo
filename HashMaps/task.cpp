#include <iostream>
#include <unordered_map>
#include <string.h>
#include <vector>
#include <chrono>
#include <fstream>

#define BUCKETS 10000

#define TEST(x, y)\
    if(x != y) \
        std::cout << "Test failed in line: " +  std::to_string(__LINE__) << \
        " got: " << x << " expected: " << y << std::endl;


template <typename V, typename K>
class HashNode
{
    private:
        K key;
        V value;
    public:
        HashNode(K &key, V &value): key(key), value(value){}
        K getKey(){return key;}
        V getValue(){return value;}
        void setValue(V value){this->value = value;}
        void setKey(K key){this->key = key;}
};

template <typename V>
class UnorderedMapInt
{
    private:
        std::array<V, BUCKETS> buckets = {0};
    public:
        void add(int key, V value)
        {
            buckets[calculateIndex(calculateHash(key))] = value;
        }

        V get(int key)
        {
            return buckets[calculateIndex(calculateHash(key))];
        }
        
        int calculateHash(int key){return key;}

        int calculateIndex(int hash){return hash % buckets.size();}
        
        void present()
        {
            for(V value: buckets)
            {
                std::cout<<value<<std::endl;
            }
        }
};

void testIntMap();
void speedTest();

int main()
{
    testIntMap();
}

void testIntMap()
{
    UnorderedMapInt<int> map1;
    std::array<int, 8> indexes = {10, 20, 25, 25, 0, 2, 3, 4};
    for(int i=0;i<indexes.size();i++)
    {
        map1.add(indexes[i], i);
    }

    for(int i=0;i<indexes.size();i++)
    {
        TEST(map1.get(indexes[i]), i);
    }
}

void speedTest()
{
    std::fstream file;
    file.open("data.dat", std::ios::in);
    int key, value;
    UnorderedMapInt<int> myMap;
    std::unordered_map<int, int> libMap;
    int sum1, sum2;
    while(file >> key >> value)
    {
        auto start1 = std::chrono::steady_clock::now();
        myMap.add(key, value);
        auto start2 = std::chrono::steady_clock::now();
        libMap[key] = value;
        auto end = std::chrono::steady_clock::now();
        sum1 += std::chrono::duration_cast<std::chrono::nanoseconds>(start2 - start1).count();
        sum2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start2).count();
    }
    file.close();
    std::cout << "writing benchmark:\nour map: " << sum1 << " ns.\nlib map: " << sum2 << " ns." << std::endl;
    std::cout << std::endl;
    
    std::array<int, 10> testValues = {1560, 1566, 99881, 67216, 2, 824661, 
        32675, 655246, 81631, 15217};

    sum1 = 0;
    sum2 = 0;
    for(int searched : testValues)
    {
        auto start1 = std::chrono::steady_clock::now();
        myMap.get(key);
        auto start2 = std::chrono::steady_clock::now();
        libMap[key];
        auto end = std::chrono::steady_clock::now();
        sum1 += std::chrono::duration_cast<std::chrono::nanoseconds>(start2 - start1).count();
        sum2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start2).count();
        if(myMap.get(key) != libMap[key]){
            std::cout<<"error"<<std::endl;
            throw(std::exception());
        }
    }
    std::cout << "searching benchmark:\nour map: " << sum1 << " ns.\nlib map: " << sum2 << " ns." << std::endl;
}
