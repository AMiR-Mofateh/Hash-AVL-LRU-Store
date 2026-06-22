#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "CacheNode.h"
#include <string>

class HashTable {
private:
    CacheNode** table; // آرایه‌ای از اشاره‌گرها به گره‌ها (همون buckets)
    int capacity;      // ظرفیت جدول هش (تعداد bucketها)
    int size;          // تعداد کل گره‌های ذخیره شده

    unsigned int hash_function(const std::string& key) const;

public:
    HashTable(int capacity);
    ~HashTable();

    CacheNode* find(const std::string& key);

    void insert(CacheNode* node);

    void remove(CacheNode* node);
    
    int getSize() const;
    int getCapacity() const;
};

#endif