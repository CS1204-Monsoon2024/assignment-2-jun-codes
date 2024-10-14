#include <iostream>
#include <cmath>

class HashTable {
private:
    int* keys;
    bool* occupied;
    int table_size;
    int current_size;
    const double load_factor_threshold = 0.8;

    bool is_prime(int n) {
        if (n < 2) return false;
        for (int i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    int next_prime(int n) {
        while (!is_prime(n)) n++;
        return n;
    }

    int hash(int key) {
        return key % table_size;
    }

    void resize_table() {
        int new_size = next_prime(table_size * 2);
        int* old_keys = keys;
        bool* old_occupied = occupied;
        int old_size = table_size;

        table_size = new_size;
        keys = new int[table_size];
        occupied = new bool[table_size];

        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            occupied[i] = false;
        }

        current_size = 0;
        for (int i = 0; i < old_size; ++i) {
            if (old_occupied[i]) {
                insert(old_keys[i]);
            }
        }

        delete[] old_keys;
        delete[] old_occupied;
    }

    int quadratic_probe(int key) {
        int index = hash(key);
        int i = 0;
        while (occupied[index]) {
            if (keys[index] == key) {
                std::cout << "Duplicate key insertion is not allowed" << std::endl;
                return -1;
            }
            i++;
            index = (hash(key) + i * i) % table_size;
            if (i > table_size / 2) {
                std::cout << "Max probing limit reached!" << std::endl;
                return -1;
            }
        }
        return index;
    }

public:
    // Constructor to initialize the hash table with a prime size
    HashTable(int size) {
        table_size = next_prime(size);
        current_size = 0;
        keys = new int[table_size];
        occupied = new bool[table_size];
        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            occupied[i] = false;
        }
    }

    // Destructor to free the allocated memory
    ~HashTable() {
        delete[] keys;
        delete[] occupied;
    }

    // Insert a key into the hash table
    void insert(int key) {
        if ((double)current_size / table_size >= load_factor_threshold) {
            resize_table();
        }

        int index = quadratic_probe(key);
        if (index != -1) {
            keys[index] = key;
            occupied[index] = true;
            current_size++;
        }
    }

    // Remove a key from the hash table
    void remove(int key) {
        int index = hash(key);
        int i = 0;
        while (occupied[index]) {
            if (keys[index] == key) {
                occupied[index] = false;
                keys[index] = -1;
                current_size--;
                return;
            }
            i++;
            index = (hash(key) + i * i) % table_size;
            if (i > table_size / 2) {
                break;
            }
        }
        std::cout << "Element not found" << std::endl;
    }

    // Search for a key and return its index
    int search(int key) {
        int index = hash(key);
        int i = 0;
        while (occupied[index]) {
            if (keys[index] == key) {
                return index;
            }
            i++;
            index = (hash(key) + i * i) % table_size;
            if (i > table_size / 2) {
                return -1;
            }
        }
        return -1;
    }

    // Print the current state of the hash table
    void printTable() {
        for (int i = 0; i < table_size; ++i) {
            if (occupied[i]) {
                std::cout << keys[i] << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
};
