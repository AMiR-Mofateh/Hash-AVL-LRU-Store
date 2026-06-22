#include "Database.h"

Database::Database(int cap) : capacity(cap), currentSize(0) {
    hashTable = new HashTable(cap); // جدول هش معمولاً متناسب با ظرفیت ساخته می‌شود
    lruList = new LRUList();
    avlTree = new AVLTree();
}

Database::~Database() {
    // اول همه گره‌ها رو از LRU list پیاده‌سازی  میشه 
    CacheNode* current = lruList->getHead();
    while (current != nullptr) {
        CacheNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    // بعد هم ساختارها
    delete hashTable;
    delete lruList;
    delete avlTree;
}

void Database::put(const std::string& key, const std::string& value) {
    CacheNode* existingNode = hashTable->find(key);

    if (existingNode != nullptr) {
        // ۱. کلید از قبل وجود داره: فقط به‌روزرسانی مقدار و انتقال به سر صف
        existingNode->value = value;
        lruList->moveToHead(existingNode);
        std::cout << "OK: Updated\n";
    } else {
        // ۲. کلید جدیده
        // اگر ظرفیت پر باشد، باید ابتدا یک گره را اخراج (Evict) کنیم
        if (currentSize == capacity) {
            CacheNode* tailNode = lruList->getTail();
            if (tailNode != nullptr) {
                std::string evictedKey = tailNode->key;
                
                // حذف از درخت AVL
                avlTree->remove(evictedKey);
                // حذف از جدول هش
                hashTable->remove(tailNode);
                // حذف از انتهای صف LRU (و آزادسازی حافظه خود گره)
                lruList->removeNode(tailNode);
                delete tailNode; 
                
                currentSize--;
            }
        }

        // درج گره جدید
        CacheNode* newNode = new CacheNode(key, value);
        hashTable->insert(newNode);
        avlTree->insert(newNode);
        lruList->addToHead(newNode);
        
        currentSize++;
        std::cout << "OK: Inserted\n";
    }
}

void Database::get(const std::string& key) {
    CacheNode* node = hashTable->find(key);
    if (node != nullptr) {
        // مقدار پیدا شد، پس به سر صف منتقل می‌شود (LRU)
        lruList->moveToHead(node);
        std::cout << "VALUE: " << node->value << "\n";
    } else {
        std::cout << "NOT FOUND\n";
    }
}

void Database::del(const std::string& key) {
    CacheNode* node = hashTable->find(key);
    if (node != nullptr) {
        // حذف از هر سه ساختار داده
        avlTree->remove(key);
        hashTable->remove(node);
        lruList->removeNode(node);
        
        delete node; // آزادسازی نهایی حافظه گره
        currentSize--;
        std::cout << "OK: Deleted\n";
    } else {
        std::cout << "NOT FOUND\n";
    }
}

void Database::range(const std::string& key1, const std::string& key2) {
    std::vector<CacheNode*> results = avlTree->rangeQuery(key1, key2);
    if (results.empty()) {
        std::cout << "EMPTY RANGE\n";
    } else {
        for (CacheNode* node : results) {
            std::cout << node->key << ":" << node->value << " ";
        }
        std::cout << "\n";
    }
}

void Database::stats() const {
    std::cout << "Count: " << currentSize << "/" << capacity << "\n";
    
    CacheNode* head = lruList->getHead();
    CacheNode* tail = lruList->getTail();
    
    std::cout << "LRU Head: " << (head ? head->key : "NULL") << "\n";
    std::cout << "LRU Tail: " << (tail ? tail->key : "NULL") << "\n";
}