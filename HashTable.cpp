#include <iostream>
#include <cmath>  // for sqrt in prime function

class HashTable {
private:
    int* keys;
    int table_size;
    int current_size;
    double load_factor_threshold;

    enum EntryType { EMPTY, OCCUPIED, DELETED };
    EntryType* status;

    bool is_prime(int n) {
        if (n < 2) return false;
        for (int i = 2; i <= std::sqrt(n); i++) {
            if (n % i == 0) return false;
        }
        return true;
    }
    //function to check next prime number using is prime
    int next_prime(int n) { 
        while (!is_prime(n)) n++;
        return n;
    }

    int hash(int key) {
        return key % table_size;
    }

    void resize_table() {
        int new_size = next_prime(table_size * 2); //atleast double and doing next prime
        int* old_keys = keys;
        EntryType* old_status = status;
        int old_size = table_size;

        table_size = new_size;
        keys = new int[table_size];
        status = new EntryType[table_size];

        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            status[i] = EMPTY;
        }

        current_size = 0;
        for (int i = 0; i < old_size; ++i) {
            if (old_status[i] == OCCUPIED) {
                insert(old_keys[i], true); // suppress output messages during resizing
            }
        }

        delete[] old_keys;
        delete[] old_status;
    }

    int quadratic_probe(int key, bool suppressMessages = false) {
        int index = hash(key);
        int i = 0;
        int initialIndex = index;

        while (status[index] == OCCUPIED) { //while loop to keep probing until condition m+1/2 is met
            if (keys[index] == key) {
                if (!suppressMessages)
                    std::cout << "Duplicate key insertion is not allowed" << std::endl;
                return -1;
            }
            i++;
            index = (initialIndex + i * i) % table_size;
            if (i > (table_size + 1) / 2) {
                if (!suppressMessages)
                    std::cout << "Max probing limit reached!" << std::endl;
                return -1;
            }
        }
        return index;
    }

public:
    HashTable(int size) {
        load_factor_threshold = 0.8;
        table_size = next_prime(size);
        keys = new int[table_size];
        status = new EntryType[table_size];
        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            status[i] = EMPTY;
        }
        current_size = 0;
    }

    ~HashTable() {
        delete[] keys;
        delete[] status;
    }

    void insert(int key, bool suppressMessages = false) {
        // checking current load factor before insertion as we only update when after threshold crossed
        if ((double)current_size / table_size >= load_factor_threshold) {
            resize_table();
        }

        int index = quadratic_probe(key, suppressMessages);
        if (index != -1) {
            keys[index] = key;
            status[index] = OCCUPIED;
            current_size++;
        }
    }

    void remove(int key) {
        int index = hash(key);
        int i = 0;
        int initialIndex = index;

        while (status[index] != EMPTY) {
            if (status[index] == OCCUPIED && keys[index] == key) {
                status[index] = DELETED;
                keys[index] = -1; // optional
                current_size--;
                return;
            }
            i++;
            index = (initialIndex + i * i) % table_size;
            if (i > (table_size + 1) / 2) {
                break;
            }
        }
        std::cout << "Element not found" << std::endl;
    }
    //keeps searching until condition for suficient probws
    int search(int key) {
        int index = hash(key);
        int i = 0;
        int initialIndex = index;

        while (status[index] != EMPTY) {
            if (status[index] == OCCUPIED && keys[index] == key) {
                return index;
            }
            i++;
            index = (initialIndex + i * i) % table_size;
            if (i > (table_size + 1) / 2) {
                return -1;
            }
        }
        return -1;
    }

    void printTable() {
        for (int i = 0; i < table_size; ++i) {
            if (status[i] == OCCUPIED) {
                std::cout << keys[i] << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
};
