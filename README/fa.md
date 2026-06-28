# ⚡ LRU Cache Database — پروژه پایانی ساختمان داده‌ها
**دانشگاه صنعتی اصفهان**

---

## 📌 معرفی پروژه

یک **In-Memory Key-Value Store** بسیار سریع، مشابه Redis، با مکانیزم **LRU Eviction** و پشتیبانی از **Range Query** با سرعت O(log n).

تمامی ساختارهای داده **From Scratch** پیاده‌سازی شده‌اند — بدون استفاده از هیچ کتابخانه یا کانتینر آماده‌ای از STL.

---

## 🏗️ معماری سیستم

```
┌─────────────────────────────────────────────────────┐
│                      DATABASE                       │
│                                                     │
│  ┌─────────────┐  ┌──────────────┐  ┌───────────┐  │
│  │  Hash Table │  │  LRU DLL     │  │ AVL Tree  │  │
│  │  O(1) avg   │  │  O(1)        │  │ O(log n)  │  │
│  │  GET/PUT/DEL│  │  Eviction    │  │ RANGE     │  │
│  └──────┬──────┘  └──────┬───────┘  └─────┬─────┘  │
│         │                │                │         │
│         └────────────────┼────────────────┘         │
│                          ↓                          │
│                   [ CacheNode* ]                    │
│         یک شیء واحد در RAM — سه ساختار              │
└─────────────────────────────────────────────────────┘
```

### الگوی Centralized Node

هر داده فقط **یک بار** در حافظه ساخته می‌شود. یک `CacheNode` همزمان در هر سه ساختار داده زندگی می‌کند:

```cpp
struct CacheNode {
    std::string key, value;

    // LRU Doubly Linked List
    CacheNode* prev;
    CacheNode* next;

    // Hash Table Chaining
    CacheNode* nextInChain;

    // AVL Tree
    CacheNode* left;
    CacheNode* right;
    int height;
};
```

---

## 🧩 ساختارهای داده پیاده‌سازی شده

### ۱. Hash Table — موتور جستجوی O(1)

- **الگوریتم هش:** djb2 (`hash * 33 + c`)
- **مدیریت collision:** Chaining با Linked List
- **ظرفیت bucket:** برابر با Capacity دیتابیس

```
bucket[0] → [nodeA] → [nodeB] → null
bucket[1] → [nodeC] → null
bucket[2] → null
bucket[3] → [nodeD] → null
```

### ۲. LRU Doubly Linked List — مدیریت تاریخچه دسترسی

```
HEAD (MRU)                          TAIL (LRU)
   ↓                                    ↓
[جدیدترین] ↔ [...] ↔ [...] ↔ [قدیمی‌ترین]
```

- **addToHead:** O(1) — درج گره جدید
- **removeNode:** O(1) — جدا کردن از هر جایی
- **moveToHead:** O(1) — به‌روزرسانی اولویت LRU
- **getTail:** O(1) — قربانی Eviction

### ۳. AVL Tree — ایندکس مرتب‌شده برای Range Query

چرخش‌های پشتیبانی‌شده:

| حالت | شرط | عملیات |
|------|-----|---------|
| **LL** | balance > 1 و left-left | rightRotate(node) |
| **RR** | balance < -1 و right-right | leftRotate(node) |
| **LR** | balance > 1 و left-right | leftRotate(left) → rightRotate(node) |
| **RL** | balance < -1 و right-left | rightRotate(right) → leftRotate(node) |

حذف با دو فرزند: جانشین (کوچکترین گره زیردرخت راست) جایگزین می‌شود بدون کپی داده — برای حفظ سلامت پیوندهای LRU و Hash.

---

## ⚙️ چرخه حیات عملیات‌ها

### PUT [key] [value]

```
کلید وجود دارد؟
    ├─ بله → value را به‌روز کن + moveToHead در LRU
    └─ خیر → ظرفیت پر است؟
                ├─ بله → Evict Tail از Hash + AVL + LRU + delete
                └─ خیر → CacheNode جدید بساز
                          ↓
                    insert به Hash + AVL + addToHead در LRU
```

### GET [key]

```
hash(key) → bucket → زنجیره را پیمایش کن
    ├─ یافت نشد → "NOT FOUND"
    └─ یافت شد → مقدار را برگردان + moveToHead در LRU
```

### DEL [key]

```
hash(key) → گره را پیدا کن
    ├─ یافت نشد → "NOT FOUND"
    └─ یافت شد → حذف از AVL + Hash + LRU + delete حافظه
```

---

## 💻 نحوه Build و اجرا

### کامپایل

```bash
g++ -std=c++17 -Wall -o lru_cache \
    main.cpp HashTable.cpp LRUList.cpp AVLTree.cpp Database.cpp
```

### کامپایل با AddressSanitizer (برای تست حافظه)

```bash
g++ -std=c++17 -fsanitize=address,leak -g -o lru_cache_debug \
    main.cpp HashTable.cpp LRUList.cpp AVLTree.cpp Database.cpp
```

### اجرا

```bash
./lru_cache
```

---

## 📟 دستورات پشتیبانی‌شده

```
Enter Cache Capacity: 5
Database initialized. Waiting for commands...
```

| دستور | توضیح | پیچیدگی |
|-------|--------|---------|
| `PUT key value` | درج یا به‌روزرسانی | O(log n) |
| `GET key` | خواندن مقدار | O(1) avg |
| `DEL key` | حذف کامل | O(log n) |
| `RANGE key1 key2` | پرس‌وجوی بازه‌ای (مرتب‌شده) | O(log n + k) |
| `STATS` | وضعیت سیستم | O(1) |
| `EXIT` | خروج | — |

---

## 🧪 نمونه اجرا

```
Enter Cache Capacity: 3
Database initialized. Waiting for commands...

PUT apple 100
OK: Inserted

PUT banana 200
OK: Inserted

PUT cherry 300
OK: Inserted

STATS
Count: 3/3
LRU Head: cherry
LRU Tail: apple

GET apple
VALUE: 100

STATS
Count: 3/3
LRU Head: apple
LRU Tail: banana

PUT date 400
OK: Inserted          ← banana (قدیمی‌ترین) evict شد

GET banana
NOT FOUND

RANGE apple cherry
apple:100 cherry:300

DEL cherry
OK: Deleted

DEL ghost
NOT FOUND
```

---

## 🔍 Edge Cases پوشش داده‌شده

| حالت | رفتار سیستم |
|------|-------------|
| ظرفیت = ۱ | به محض PUT دوم، اولی evict می‌شود |
| کلید تکراری (Overwrite) | گره جدید ساخته **نمی‌شود**، فقط value و LRU به‌روز می‌شود |
| کلید ناموجود در GET/DEL | پیام `NOT FOUND` — بدون crash |
| RANGE خارج از محدوده | پیام `EMPTY RANGE` |
| RANGE معکوس (key1 > key2) | به‌صورت خودکار swap می‌شود |
| حذف از Head | پیوند Head به‌روز می‌شود |
| حذف از Tail | پیوند Tail به‌روز می‌شود |
| DEL + PUT همان کلید | بدون تداخل اشاره‌گر |
| خالی کردن کامل | Head=NULL, Tail=NULL |
| دستور نامعتبر | پیام `UNKNOWN COMMAND` — بدون توقف |
| Eviction از وسط AVL | جانشین‌گذاری صحیح، درخت balanced می‌ماند |

---

## 📁 ساختار فایل‌ها

```
.
├── CacheNode.h        ← تعریف گره مشترک (داده + اشاره‌گرهای سه ساختار)
├── HashTable.h/.cpp   ← Hash Table با Chaining
├── LRUList.h/.cpp     ← Doubly Linked List (مدیریت LRU)
├── AVLTree.h/.cpp     ← AVL Tree (Range Query)
├── Database.h/.cpp    ← یکپارچه‌ساز سه ساختار + منطق Eviction
└── main.cpp           ← حلقه CLI و پردازش دستورات
```

---

## 📊 پیچیدگی زمانی

| عملیات | Hash Table | LRU List | AVL Tree | کل |
|--------|-----------|----------|----------|----|
| PUT (جدید) | O(1) avg | O(1) | O(log n) | **O(log n)** |
| PUT (update) | O(1) avg | O(1) | — | **O(1) avg** |
| GET | O(1) avg | O(1) | — | **O(1) avg** |
| DEL | O(1) avg | O(1) | O(log n) | **O(log n)** |
| RANGE | — | — | O(log n + k) | **O(log n + k)** |
| Eviction | O(1) avg | O(1) | O(log n) | **O(log n)** |

> k = تعداد نتایج بازه‌ای

---

## ⚠️ نکات مهم پیاده‌سازی

**مدیریت حافظه:**
- مالکیت تمام `CacheNode` ها با کلاس `Database` است
- در `Database::~Database()` ابتدا همه گره‌ها از طریق LRU list پاکسازی می‌شوند
- `HashTable::~HashTable()` و `LRUList::~LRUList()` و `AVLTree::~AVLTree()` خود گره‌ها را delete **نمی‌کنند**

**حذف دو فرزند در AVL:**
- به جای کپی key/value (که پیوندهای LRU و Hash را خراب می‌کند)، گره جانشین فیزیکاً جایگزین می‌شود

**Hash Table Capacity:**
- تعداد bucket‌ها برابر با Capacity دیتابیس است
- در صورت load factor بالا، collision‌ها از طریق زنجیره مدیریت می‌شوند
