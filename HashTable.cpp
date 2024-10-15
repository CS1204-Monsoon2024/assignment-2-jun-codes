#include <iostream>
#include <cmath>  // for sqrt in is_prime

class HashTable {
private:
    int* keys;
    int table_size;
    int current_size;
    double load_factor_threshold;

    enum EntryType { EMPTY, OCCUPIED, DELETED };
    EntryType* status;

    bool is_prime(int n) {
        // check if n is a prime number
        if (n < 2) return false;
        for (int i = 2; i <= std::sqrt(n); i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    int next_prime(int n) {
        // finding next prime number after n
        while (!is_prime(n)) n++;
        return n;
    }

    int hash(int key) {
        return key % table_size;
    }

    void resize_table() {
        // resize table when load factor exceeds threshold (only after the insert tho)
        int new_size = next_prime(table_size * 2);
        int* old_keys = keys;
        EntryType* old_status = status;
        int old_size = table_size;

        // updating to new sizee
        table_size = new_size;
        keys = new int[table_size];
        status = new EntryType[table_size];

        
        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            status[i] = EMPTY;
        }

        current_size = 0;
        // re-insert old keys into the new table(kinda rehash)
        for (int i = 0; i < old_size; ++i) {
            if (old_status[i] == OCCUPIED) {
                insert(old_keys[i], true); // suppress outputs when resizing
            }
        }

        // clean up old arrays
        delete[] old_keys;
        delete[] old_status;
    }

    int quadratic_probe(int key, bool suppressMessages = false) {
        // finding indes using quad probe
        int index = hash(key);
        int i = 0;
        int initialIndex = index;

        while (status[index] == OCCUPIED) {
            if (keys[index] == key) {
                if (!suppressMessages)
                    std::cout << "duplicate key insertion is not allowed" << std::endl;
                return -1;
            }
            i++;
            index = (initialIndex + i * i) % table_size;
            // check if we've probed too many times
            if (i > (table_size + 1) / 2) {
                if (!suppressMessages)
                    std::cout << "max probing limit reached!" << std::endl;
                return -1;
            }
        }
        return index;
    }

public:
    HashTable(int size) {
        //initialize the hash table
        load_factor_threshold = 0.8;
        table_size = next_prime(size);
        keys = new int[table_size];
        status = new EntryType[table_size];
        // set all slots to empty
        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            status[i] = EMPTY;
        }
        current_size = 0;
    }

    ~HashTable() {
        // to clean up memory
        delete[] keys;
        delete[] status;
    }

    void insert(int key, bool suppressMessages = false) {
        // check current load factor before inserting
        if ((double)current_size / table_size >= load_factor_threshold) {
            resize_table();
        }

        int index = quadratic_probe(key, suppressMessages);
        if (index != -1) {
            // insert key into the table
            keys[index] = key;
            status[index] = OCCUPIED;
            current_size++;
        }
    }

    void remove(int key) {
        // remove a key from the table
        int index = hash(key);
        int i = 0;
        int initialIndex = index;

        while (status[index] != EMPTY) {
            if (status[index] == OCCUPIED && keys[index] == key) {
                // mark the slot as deleted
                status[index] = DELETED;
                keys[index] = -1; 
                current_size--;
                return;
            }
            i++;
            index = (initialIndex + i * i) % table_size;
            // check if we've probed too many times
            if (i > (table_size + 1) / 2) {
                break;
            }
        }
        std::cout << "element not found" << std::endl;
    }

    int search(int key) {
        // search for a key in the table
        int index = hash(key);
        int i = 0;
        int initialIndex = index;

        while (status[index] != EMPTY) {
            if (status[index] == OCCUPIED && keys[index] == key) {
                return index; // key found
            }
            i++;
            index = (initialIndex + i * i) % table_size;
            // check if we've probed too many times
            if (i > (table_size + 1) / 2) {
                return -1; // key not found
            }
        }
        return -1; // key not found
    }

    void printTable() {
        // print current state of the hash table
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
