// LRUList.h
#ifndef LRULIST_H
#define LRULIST_H

#include "CacheNode.h"

class LRUList {
private:
    CacheNode* head; // (Most Recently Used)
    CacheNode* tail; // (Least Recently Used)

public:
    LRUList();
    ~LRUList(); 

    void addToHead(CacheNode* node);     // افزودن گره جدید به ابتدای لیست
    void removeNode(CacheNode* node);    // جدا کردن یک گره از لیست
    void moveToHead(CacheNode* node);    // انتقال یک گره موجود به ابتدای لیست (برای GET/PUT)
    
    CacheNode* getTail() const;          // دریافت گره Tail (برای Eviction)
    CacheNode* getHead() const;          // دریافت گره Head (برای Stats)
};

#endif // LRULIST_H