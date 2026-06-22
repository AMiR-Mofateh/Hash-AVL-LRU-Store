#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "CacheNode.h"
#include "HashTable.h"
#include "LRUList.h"
#include "AVLTree.h"

class Database {
private:
    int capacity;
    int currentSize;
    
    HashTable* hashTable;
    LRUList* lruList;
    AVLTree* avlTree;

public:
    Database(int cap);
    ~Database();

    void put(const std::string& key, const std::string& value);
    void get(const std::string& key);
    void del(const std::string& key);
    void range(const std::string& key1, const std::string& key2);
    void stats() const;
};

#endif 