#include "LRUList.h"

LRUList::LRUList() {
    head = nullptr;
    tail = nullptr;
}

LRUList::~LRUList() {
    // حافظه گره‌ها توسط Database مدیریت می‌شود، در اینجا نیازی به حذف گره‌ها نداریم.
}

void LRUList::addToHead(CacheNode* node) {
    if (!node) return;

    node->prev = nullptr;
    node->next = head;

    if (head != nullptr) {
        head->prev = node;
    }
    head = node;

    // اگر لیست خالی بوده، گره جدید همزمان tail هم هست
    if (tail == nullptr) {
        tail = head;
    }
}

void LRUList::removeNode(CacheNode* node) {
    if (!node) return;

    // مدیریت اتصال گره قبلی
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        // اگر گره قبلی ندارد، یعنی خودش head است
        head = node->next;
    }

    // مدیریت اتصال گره بعدی
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        // اگر گره بعدی ندارد، یعنی خودش tail است
        tail = node->prev;
    }

    // پاک‌سازی اشاره‌گرهای گره جدا شده (برای ایمنی)
    node->prev = nullptr;
    node->next = nullptr;
}

void LRUList::moveToHead(CacheNode* node) {
    if (!node || node == head) return; // اگر از قبل در head است، کاری نکن
    
    removeNode(node); // ابتدا از جای فعلی حذفش کن
    addToHead(node);  // سپس به head اضافه‌اش کن
}

CacheNode* LRUList::getTail() const {
    return tail;
}

CacheNode* LRUList::getHead() const {
    return head;
}