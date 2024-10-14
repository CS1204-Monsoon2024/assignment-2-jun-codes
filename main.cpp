#include "HashTable.cpp"

int main() {
    int initialSize = 7;
    HashTable ht(initialSize); // Create HashTable object

    ht.insert(1);
    ht.printTable();  // Print hash table contents
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

    int find = ht.search(22); // Search for key 22
    std::cout << "Found at: " << find << std::endl;

    return 0;
}
