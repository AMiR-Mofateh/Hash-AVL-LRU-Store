#ifndef CACHENODE_H
#define CACHENODE_H

#include <string>

struct CacheNode {
    std::string key;
    std::string value;

    // --- اشاره‌گرهای مربوط به LRU (Doubly Linked List) ---
    CacheNode* prev;
    CacheNode* next;

    // --- اشاره‌گرهای مربوط به Collision در Hash Table ---
    CacheNode* nextInChain;

    // --- اشاره‌گرها و اطلاعات مربوط به AVL Tree ---
    CacheNode* left;
    CacheNode* right;
    int height;

    CacheNode(const std::string& k, const std::string& v) {
        key = k;
        value = v;
        
        prev = nullptr;
        next = nullptr;
        
        nextInChain = nullptr;
        
        left = nullptr;
        right = nullptr;
        height = 1; 
    }
};

#endif