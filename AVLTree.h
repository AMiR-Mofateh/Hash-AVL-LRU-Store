#ifndef AVLTREE_H
#define AVLTREE_H

#include "CacheNode.h"
#include <string>
#include <vector>
#include <algorithm>

class AVLTree {
private:
    CacheNode* root;

    // توابع  ارتفاع و ضریب بالانس
    int height(CacheNode* node) const;
    void updateHeight(CacheNode* node);
    int getBalanceFactor(CacheNode* node) const;

    // توابع چرخش (Rotations)
    CacheNode* rightRotate(CacheNode* y);
    CacheNode* leftRotate(CacheNode* x);
    
    // تابع برای بالانس کردن یک گره
    CacheNode* rebalance(CacheNode* node);

    // توابع بازگشتی اصلی
    CacheNode* insertRecursive(CacheNode* node, CacheNode* newNode);
    CacheNode* removeRecursive(CacheNode* node, const std::string& key);
    CacheNode* findMinNode(CacheNode* node) const;

    // پیمایش برای جستجوی بازه‌ای (Range Query)
    void rangeQueryRecursive(CacheNode* node, const std::string& key1, const std::string& key2, std::vector<CacheNode*>& result) const;

public:
    AVLTree();
    ~AVLTree();

    void insert(CacheNode* newNode);
    void remove(const std::string& key);
    
    // برگردوندن لیستی از گره‌ها تو بازه [key1, key2]
    std::vector<CacheNode*> rangeQuery(const std::string& key1, const std::string& key2) const;
};

#endif