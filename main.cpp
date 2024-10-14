#include <iostream>
#include "HashTable.cpp"  // Ensure the class is included

int main() {
    HashTable ht(7);  // Create a HashTable object with an initial size of 7

    ht.insert(1);
    ht.printTable();

    ht.insert(6);
    ht.printTable();

    ht.insert(15);
    ht.printTable();

    ht.insert(25);
    ht.printTable();

    ht.remove(15);
    ht.printTable();

    ht.insert(29);
    ht.printTable();

    int find = ht.search(22);
    std::cout << "Found at: " << find << std::endl;

    return 0;
}
