#include "AVLTree.h"

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    // حافظه گره‌ها توسط Database مدیریت می‌شه، در اینجا نیازی به حذف گره‌ها نداریم.
}

int AVLTree::height(CacheNode* node) const {
    if (node == nullptr) return 0;
    return node->height;
}

void AVLTree::updateHeight(CacheNode* node) {
    if (node != nullptr) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

int AVLTree::getBalanceFactor(CacheNode* node) const {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

// چرخش به راست (LL Rotation)
CacheNode* AVLTree::rightRotate(CacheNode* y) {
    CacheNode* x = y->left;
    CacheNode* T2 = x->right;

    // انجام چرخش
    x->right = y;
    y->left = T2;

    // به‌روزرسانی ارتفاع‌ها
    updateHeight(y);
    updateHeight(x);

    // x ریشه جدید است
    return x;
}

// چرخش به چپ (RR Rotation)
CacheNode* AVLTree::leftRotate(CacheNode* x) {
    CacheNode* y = x->right;
    CacheNode* T2 = y->left;

    // انجام چرخش
    y->left = x;
    x->right = T2;

    // به‌روزرسانی ارتفاع‌ها
    updateHeight(x);
    updateHeight(y);

    // y ریشه جدید است
    return y;
}

CacheNode* AVLTree::rebalance(CacheNode* node) {
    updateHeight(node);
    int balance = getBalanceFactor(node);

    // حالت Left Left (LL)
    if (balance > 1 && getBalanceFactor(node->left) >= 0)
        return rightRotate(node);

    // حالت Left Right (LR)
    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // حالت Right Right (RR)
    if (balance < -1 && getBalanceFactor(node->right) <= 0)
        return leftRotate(node);

    // حالت Right Left (RL)
    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(CacheNode* newNode) {
    // مقداردهی اولیه اشاره‌گرهای درختی گره جدید
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1;
    root = insertRecursive(root, newNode);
}

CacheNode* AVLTree::insertRecursive(CacheNode* node, CacheNode* newNode) {
    if (node == nullptr)
        return newNode;

    if (newNode->key < node->key)
        node->left = insertRecursive(node->left, newNode);
    else if (newNode->key > node->key)
        node->right = insertRecursive(node->right, newNode);
    else
        return node; // کلیدهای تکراری مجاز نیستند

    return rebalance(node);
}

CacheNode* AVLTree::findMinNode(CacheNode* node) const {
    CacheNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

void AVLTree::remove(const std::string& key) {
    root = removeRecursive(root, key);
}

CacheNode* AVLTree::removeRecursive(CacheNode* node, const std::string& key) {
    if (node == nullptr)
        return node;

    if (key < node->key)
        node->left = removeRecursive(node->left, key);
    else if (key > node->key)
        node->right = removeRecursive(node->right, key);
    else {
        // گره پیدا شد
        if ((node->left == nullptr) || (node->right == nullptr)) {
            // گره دارای صفر یا یک فرزنده
            CacheNode* temp = node->left ? node->left : node->right;
            
            // ما حافظه را delete نمی‌کنیم، فقط گره را از درخت کنار می‌گذاریم (Bypass)
            return temp; 
        } else {
            // گره  دو فرزند داره
            // پیدا کردن جانشین (کوچکترین گره تو زیردرخت راست)
            CacheNode* temp = findMinNode(node->right);

            // به جای کپی کردن مقادیر (که پیوندهای LRU/Hash را خراب می‌کن)،
            // جانشین را از زیردرخت راست حذف می‌کنیم و اونو جایگزین گره فعلی می‌کنیم
            node->right = removeRecursive(node->right, temp->key);
            
            // جانشین حالا آزاد است، آن را دقیقاً تو جای node قرار می‌دهیم
            temp->left = node->left;
            temp->right = node->right;
            temp->height = 1 + std::max(height(temp->left), height(temp->right));
            return rebalance(temp);
        }
    }

    if (node == nullptr)
        return node;

    return rebalance(node);
}

std::vector<CacheNode*> AVLTree::rangeQuery(const std::string& key1, const std::string& key2) const {
    std::vector<CacheNode*> result;
    // فرض می‌کنیم key1 همیشه کوچکتر یا مساوی key2 است. 
    // اگر کاربر برعکس وارد کرد، میشه اینجا اونا رو جابجا کرد:
    std::string start = std::min(key1, key2);
    std::string end = std::max(key1, key2);
    
    rangeQueryRecursive(root, start, end, result);
    return result;
}

void AVLTree::rangeQueryRecursive(CacheNode* node, const std::string& key1, const std::string& key2, std::vector<CacheNode*>& result) const {
    if (node == nullptr) return;

    // پیمایش In-Order برای به دست آوردن خروجی مرتب شده
    // اگر کلید فعلی بزرگتر از کف بازه است، سمت چپ را بررسی کن
    if (node->key > key1) {
        rangeQueryRecursive(node->left, key1, key2, result);
    }

    // اگر کلید فعلی داخل بازه است، اونو به لیست اضافه کن
    if (node->key >= key1 && node->key <= key2) {
        result.push_back(node);
    }

    // اگر کلید فعلی کوچکتر از سقف بازه است، سمت راست زو بررسی کن
    if (node->key < key2) {
        rangeQueryRecursive(node->right, key1, key2, result);
    }
}
