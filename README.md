# ⚡ In-Memory Key-Value Store — Hash Table • LRU Cache • AVL Tree

<a![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
<a![Build](https://img.shields.io/badge/build-g%2B%2B-lightgrey.svg)
<a![Structures](https://img.shields.io/badge/data%20structures-Hash%20Table%20%7C%20DLL%20%7C%20AVL%20Tree-orange.svg)
<a![Status](https://img.shields.io/badge/status-university%20project-green.svg)
<p align="center">

<a href="https://amir-mofateh.github.io/Hash-AVL-LRU-Store/">
<img src="https://img.shields.io/badge/🚀_Launch-Live_Demo-success?style=for-the-badge"/>
</a>

<a href="https://github.com/AMiR-Mofateh/Hash-AVL-LRU-Store">
<img src="https://img.shields.io/badge/⭐_Star_this_Repository-GitHub-black?style=for-the-badge&logo=github"/>
</a>

</p>

<p align="center">
<b>🎮 Try the interactive visualization directly in your browser — no installation required.</b>
</p>
A high-performance, **from-scratch** in-memory key-value store in C++, built for the Data Structures course at Isfahan University of Technology. It combines a custom hash table, a doubly linked list, and a self-balancing AVL tree around a single shared node type, giving O(1)-average lookups, O(1) LRU eviction, and O(log n + k) sorted range queries — with zero STL containers.

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Features](#features)
- [Getting Started](#getting-started)
- [Command Reference](#command-reference)
- [Time Complexity](#time-complexity)
- [Edge Cases Handled](#edge-cases-handled)
- [Testing](#testing)
- [Project Structure](#project-structure)
- [Known Limitations / Roadmap](#known-limitations--roadmap)

## Overview

This project implements a fixed-capacity, Redis-like in-memory store:

- **`PUT key value`** inserts or updates a key.
- **`GET key`** retrieves a value and marks the key as recently used.
- **`DEL key`** removes a key.
- **`RANGE key1 key2`** returns all keys in a sorted range.
- When the store is full, the **Least Recently Used (LRU)** key is automatically evicted to make room.

All three underlying data structures — hash table, doubly linked list, and AVL tree — are implemented manually, without `std::unordered_map`, `std::list`, or `std::map`.

## Architecture

The core design decision is a **single shared node** (`CacheNode`) that simultaneously belongs to all three structures, so every piece of data is stored exactly once in RAM:

```
┌─────────────────────────────────────────────────────┐
│                      DATABASE                        │
│                                                       │
│  ┌─────────────┐   ┌──────────────┐   ┌───────────┐  │
│  │  Hash Table │   │  LRU DLL     │   │ AVL Tree  │  │
│  │  O(1) avg   │   │  O(1)        │   │ O(log n)  │  │
│  │  find       │   │  eviction    │   │ RANGE     │  │
│  └──────┬──────┘   └──────┬───────┘   └─────┬─────┘  │
│         │                 │                 │        │
│         └─────────────────┼─────────────────┘        │
│                           ↓                           │
│                    [ CacheNode* ]                     │
│         one object in RAM, linked three ways          │
└─────────────────────────────────────────────────────┘
```

```cpp
struct CacheNode {
    std::string key, value;

    CacheNode* prev;        // LRU: doubly linked list
    CacheNode* next;

    CacheNode* nextInChain; // Hash table: collision chaining

    CacheNode* left;        // AVL tree
    CacheNode* right;
    int height;
};
```

## Features

- **Fast lookups** — average O(1) `GET`/`PUT`/`DEL`, powered by a custom hash table with chaining for collision resolution.
- **LRU eviction** — when the cache is full, the least recently used entry is evicted in O(1) using a custom doubly linked list.
- **Sorted range queries** — `RANGE key1 key2` returns keys in sorted order in O(log n + k) using a self-balancing AVL tree, including automatic handling of reversed bounds (`key1 > key2`).
- **Safe deletion in the AVL tree** — when a node with two children is removed, the in-order successor is physically relocated into its place instead of having its key/value copied, which keeps the LRU and hash-table pointers embedded in that node intact.
- **CLI interface** — runs interactively or by piping commands from a file.

## Getting Started

### Prerequisites

- A C++ compiler supporting C++11 or later (e.g. `g++`, `clang++`).

### Compilation

```bash
g++ -std=c++17 -Wall -Wextra -o cachedb main.cpp Database.cpp HashTable.cpp LRUList.cpp AVLTree.cpp
```

### Running

**Interactive mode:**

```bash
./cachedb
```

**From a file:**

```bash
# tests.txt
# 3
# PUT apple 100
# GET apple
# EXIT

./cachedb < tests.txt
```

## Command Reference

| Command | Syntax | Description |
|---|---|---|
| `PUT` | `PUT <key> <value>` | Inserts or updates a key. Triggers eviction if the store is full. |
| `GET` | `GET <key>` | Retrieves the value for a key and marks it as most recently used. Prints `NOT FOUND` if missing. |
| `DEL` | `DEL <key>` | Deletes a key-value pair. Prints `NOT FOUND` if missing. |
| `RANGE` | `RANGE <key1> <key2>` | Prints all keys in `[min(key1,key2), max(key1,key2)]`, sorted. Prints `EMPTY RANGE` if none match. |
| `STATS` | `STATS` | Prints item count, LRU head key, and LRU tail key. |
| `EXIT` | `EXIT` | Terminates the program. |

## Time Complexity

| Operation | Hash Table | LRU List | AVL Tree | Overall |
|---|---|---|---|---|
| `PUT` (new key) | O(1) avg | O(1) | O(log n) | **O(log n)** |
| `PUT` (update) | O(1) avg | O(1) | — | **O(1) avg** |
| `GET` | O(1) avg | O(1) | — | **O(1) avg** |
| `DEL` | O(1) avg | O(1) | O(log n) | **O(log n)** |
| `RANGE` | — | — | O(log n + k) | **O(log n + k)** |
| Eviction | O(1) avg | O(1) | O(log n) | **O(log n)** |

*k = number of results returned by a range query.*

## Edge Cases Handled

| Case | Behavior |
|---|---|
| Capacity = 1 | The existing entry is evicted the moment a second key is inserted. |
| Overwriting an existing key | No new node is created — only the value and LRU position are updated. |
| `GET`/`DEL` on a missing key | Prints `NOT FOUND`, no crash. |
| `RANGE` with no matches | Prints `EMPTY RANGE`. |
| `RANGE` with reversed bounds (`key1 > key2`) | Bounds are swapped automatically. |
| Deleting the head or the tail | LRU head/tail pointers are updated correctly. |
| `DEL` then `PUT` the same key | No dangling or duplicated pointers. |
| Emptying the store completely | Head and tail both become `NULL`. |
| Unknown command | Prints `UNKNOWN COMMAND`, the program keeps running. |
| Eviction of a node in the middle of the AVL tree | Successor replacement keeps the tree balanced. |

## Testing

Two ready-made scenario files are included:

```bash
./cachedb < tests.txt
./cachedb < tests2.txt
```

Both exercise insertion, updates, eviction under capacity pressure, deletion, and both forward and reversed range queries.

## Project Structure

```
.
├── CacheNode.h        # Shared node: data + pointers for all three structures
├── HashTable.h/.cpp   # Hash table with chaining
├── LRUList.h/.cpp     # Doubly linked list (LRU ordering)
├── AVLTree.h/.cpp     # AVL tree (range queries)
├── Database.h/.cpp    # Integration layer + eviction logic
├── main.cpp           # CLI loop and command parsing
├── tests.txt           # Sample test scenario
└── tests2.txt          # Sample test scenario
```

## Known Limitations / Roadmap

- **Transactions are not yet implemented.** A natural extension would be `BEGIN` / `COMMIT` / `ROLLBACK`, backed by a custom stack that logs the inverse of each `PUT`/`DEL` so `ROLLBACK` can replay it LIFO. This is left as future work.
- Memory ownership is centralized in `Database`; the hash table, LRU list, and AVL tree only hold pointers into nodes that `Database` allocates and frees.
