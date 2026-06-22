# In-Memory Key-Value Store with LRU Cache & AVL Tree

This project is a C++ implementation of a high-performance in-memory key-value store. It features a fixed-capacity cache with an LRU (Least Recently Used) eviction policy and supports efficient range queries using a self-balancing AVL tree. A key constraint of this project is that all fundamental data structures (Hash Table, Doubly Linked List, AVL Tree) are built from scratch.

## Core Features

-   **Fast Lookups:** Average $O(1)$ time complexity for `GET`, `PUT`, and `DEL` operations, powered by a custom hash table with chaining for collision resolution.
-   **LRU Eviction Policy:** When the cache reaches full capacity, the least recently used item is automatically evicted to make space for new data. This is managed efficiently in $O(1)$ time using a custom doubly linked list.
-   **Efficient Range Queries:** Supports sorted key retrieval within a given range in $O(\log n + k)$ time, where $k$ is the number of elements in the range, thanks to a custom self-balancing AVL tree.
-   **Transactional Support (Bonus):** Implements `BEGIN`, `COMMIT`, and `ROLLBACK` commands for atomic operations using a custom stack for undo logs.
-   **Command-Line Interface:** Can be controlled interactively or by redirecting input from a file.

## Architecture & Data Structures

The system's efficiency stems from a unified node structure, `CacheNode`, which is simultaneously part of three different data structures:

1.  **Hash Table:** A custom-built hash table maps keys to `CacheNode` pointers, enabling average $O(1)$ access. Collisions are handled using the chaining method, where nodes with the same hash index are linked together.
2.  **Doubly Linked List:** This list maintains the usage order for the LRU policy. The head of the list is the most recently used item, and the tail is the least recently used. All updates to this list (moving a node to the head, removing the tail) are $O(1)$ operations.
3.  **AVL Tree:** A self-balancing binary search tree stores all `CacheNode`s based on their keys. This structure guarantees that insertions, deletions, and searches are performed in $O(\log n)$ time, and enables efficient range queries.

This integrated approach avoids data duplication and ensures consistency across all operations.

## How to Get Started

### Prerequisites
- A C++ compiler that supports C++11 or later (e.g., g++, Clang).

### Compilation
Navigate to the project's root directory and compile all the `.cpp` source files using the following command:

```bash
g++ main.cpp Database.cpp AVLTree.cpp HashTable.cpp LRUList.cpp -o my_database

### Running the Program

You can run the program in two modes:

**1. Interactive Mode:**
Simply execute the compiled binary. The program will prompt for capacity and then wait for your commands.

bash
./my_database

**2. From a File:**
Create a text file (e.g., `tests.txt`) with a list of commands, starting with the capacity. Then, use input redirection to execute them.

bash
# Example tests.txt content:
# 3
# PUT apple 100
# GET apple
# EXIT

./my_database < tests.txt

## Command Reference

| Command        | Syntax                         | Description                                                              |
|----------------|--------------------------------|--------------------------------------------------------------------------|
| `PUT`          | `PUT <key> <value>`            | Inserts or updates a key with a given value. Triggers eviction if full.  |
| `GET`          | `GET <key>`                    | Retrieves the value for a key. Prints `(nil)` if not found.              |
| `DEL`          | `DEL <key>`                    | Deletes a key-value pair from the database.                              |
| `RANGE`        | `RANGE <start_key> <end_key>`  | Prints all keys between `start_key` and `end_key` (inclusive).           |
| `STATS`        | `STATS`                        | Displays database statistics (item count, LRU head, and LRU tail).       |
| `BEGIN`        | `BEGIN`                        | Starts a new transaction block.                                          |
| `COMMIT`       | `COMMIT`                       | Makes all changes since the last `BEGIN` permanent.                      |
| `ROLLBACK`     | `ROLLBACK`                     | Discards all changes made since the last `BEGIN`.                        |
| `EXIT`         | `EXIT`                         | Terminates the program.                                                  |

`
