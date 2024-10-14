#include <iostream>
#include <cmath>

class HashTable {
private:
    int* keys;
    enum SlotState { EMPTY, OCCUPIED, DELETED };
    SlotState* slot_state;
    int table_size;
    int current_size;
    double load_factor_threshold;

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
        int h = key % table_size;
        if (h < 0) h += table_size;
        return h;
    }

    void resize_table() {
        int new_size = next_prime(table_size * 2);
        int* old_keys = keys;
        SlotState* old_slot_state = slot_state;
        int old_size = table_size;

        table_size = new_size;
        keys = new int[table_size];
        slot_state = new SlotState[table_size];

        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            slot_state[i] = EMPTY;
        }

        current_size = 0;
        for (int i = 0; i < old_size; ++i) {
            if (old_slot_state[i] == OCCUPIED) {
                insert(old_keys[i]);
            }
        }

        delete[] old_keys;
        delete[] old_slot_state;
    }

public:
    HashTable(int size) {
        load_factor_threshold = 0.8;
        table_size = next_prime(size);
        current_size = 0;
        keys = new int[table_size];
        slot_state = new SlotState[table_size];
        for (int i = 0; i < table_size; ++i) {
            keys[i] = -1;
            slot_state[i] = EMPTY;
        }
    }

    ~HashTable() {
        delete[] keys;
        delete[] slot_state;
    }

    void insert(int key) {
        if ((double)current_size / table_size >= load_factor_threshold) {
            resize_table();
        }

        int initial_index = hash(key);
        int i = 0;
        int index;
        int first_deleted = -1;

        while (true) {
            index = (initial_index + i * i) % table_size;
            if (index < 0) index += table_size;

            if (slot_state[index] == EMPTY) {
                break;
            } else if (slot_state[index] == OCCUPIED) {
                if (keys[index] == key) {
                    std::cout << "Duplicate key insertion is not allowed" << std::endl;
                    return;
                }
            } else if (slot_state[index] == DELETED) {
                if (first_deleted == -1) {
                    first_deleted = index;
                }
            }

            i++;
            if (i >= table_size) {
                std::cout << "Max probing limit reached!" << std::endl;
                return;
            }
        }

        if (first_deleted != -1) {
            index = first_deleted;
        }

        keys[index] = key;
        slot_state[index] = OCCUPIED;
        current_size++;
    }

    void remove(int key) {
        int initial_index = hash(key);
        int i = 0;
        int index;

        while (true) {
            index = (initial_index + i * i) % table_size;
            if (index < 0) index += table_size;

            if (slot_state[index] == EMPTY) {
                std::cout << "Element not found" << std::endl;
                return;
            } else if (slot_state[index] == OCCUPIED && keys[index] == key) {
                slot_state[index] = DELETED;
                keys[index] = -1;
                current_size--;
                return;
            }

            i++;
            if (i >= table_size) {
                std::cout << "Element not found" << std::endl;
                return;
            }
        }
    }

    int search(int key) {
        int initial_index = hash(key);
        int i = 0;
        int index;

        while (true) {
            index = (initial_index + i * i) % table_size;
            if (index < 0) index += table_size;

            if (slot_state[index] == EMPTY) {
                return -1;
            } else if (slot_state[index] == OCCUPIED && keys[index] == key) {
                return index;
            }

            i++;
            if (i >= table_size) {
                return -1;
            }
        }
    }

    void printTable() {
        for (int i = 0; i < table_size; ++i) {
            if (slot_state[i] == OCCUPIED) {
                std::cout << keys[i] << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
};
