#include <iostream>
#include <string>
#include "Database.h"

int main() {
    int capacity;
    std::cout << "Enter Cache Capacity: ";
    if (!(std::cin >> capacity) || capacity <= 0) {
        std::cerr << "Invalid capacity!\n";
        return 1;
    }

    Database db(capacity);
    std::string command;

    std::cout << "Database initialized. Waiting for commands...\n";

    while (std::cin >> command) {
        if (command == "PUT") {
            std::string key, value;
            std::cin >> key >> value;
            db.put(key, value);
        } 
        else if (command == "GET") {
            std::string key;
            std::cin >> key;
            db.get(key);
        } 
        else if (command == "DEL") {
            std::string key;
            std::cin >> key;
            db.del(key);
        } 
        else if (command == "RANGE") {
            std::string key1, key2;
            std::cin >> key1 >> key2;
            db.range(key1, key2);
        } 
        else if (command == "STATS") {
            db.stats();
        } 
        else if (command == "EXIT") {
            break; 
        } 
        else {
            std::cout << "UNKNOWN COMMAND\n";
        }
    }

    return 0;
}