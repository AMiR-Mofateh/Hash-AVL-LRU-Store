#include "HashTable.h"

unsigned int HashTable::hash_function(const std::string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % capacity;
}

HashTable::HashTable(int capacity) : capacity(capacity), size(0) {
    table = new CacheNode*[capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr; 
    }
}

HashTable::~HashTable() {
    // نکته: اینجا خود گره‌ها را delete نمی‌کنیم.
    // حافظه آنها در کلاس اصلی Database مدیریت می‌شود.
    delete[] table;
}

CacheNode* HashTable::find(const std::string& key) {
    unsigned int index = hash_function(key);
    CacheNode* current = table[index];

    // پیمایش زنجیره در bucket مربوطه
    while (current != nullptr) {
        if (current->key == key) {
            return current; // گره پیدا شه
        }
        current = current->nextInChain;
    }

    return nullptr; //گره پیدا نشه 
}

void HashTable::insert(CacheNode* node) {
    if (!node) return;

    unsigned int index = hash_function(node->key);
    
    // گره جدید رو به اول زنجیره اضافه می‌کنیم
    node->nextInChain = table[index];
    table[index] = node;
    
    size++;
}

void HashTable::remove(CacheNode* node) {
    if (!node) return;

    unsigned int index = hash_function(node->key);
    CacheNode* current = table[index];
    CacheNode* prev = nullptr;

    // پیمایش برای پیدا کردن گره و گره قبلی اون تو زنجیره
    while (current != nullptr && current != node) {
        prev = current;
        current = current->nextInChain;
    }

    // اگر گره در زنجیره پیدا نشه 
    if (current == nullptr) return;

    // حذف گره از زنجیره
    if (prev == nullptr) {
        // گره اول زنجیره
        table[index] = node->nextInChain;
    } else {
        // گره  وسط یا اخر زنجیره 
        prev->nextInChain = node->nextInChain;
    }
    
    // اشاره‌گر گره حذف شده را برای ایمنی پاک می‌کنیم
    node->nextInChain = nullptr; 
    size--;
}

int HashTable::getSize() const {
    return size;
}

int HashTable::getCapacity() const {
    return capacity;
}