#include <iostream>
#include <vector>
#include <climits>

class HashTable {
private:
    std::vector<int> table;
    std::vector<bool> occupied; // To track which slots are currently occupied
    std::vector<bool> everUsed; // To track slots ever used to handle deletion properly
    int currentSize;
    int numElements;

    int hash(int key) const {
        return key % currentSize;
    }

    int nextPrime(int n) {
        if (n <= 1) return 2;
        int prime = n;
        bool found = false;

        while (!found) {
            prime++;
            if (isPrime(prime))
                found = true;
        }
        return prime;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6)
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        return true;
    }

    void resize() {
        int oldSize = currentSize;
        std::vector<int> oldTable = table;
        std::vector<bool> oldOccupied = occupied;

        currentSize = nextPrime(2 * currentSize);
        table.clear();
        table.resize(currentSize, INT_MIN);
        occupied.clear();
        occupied.resize(currentSize, false);
        everUsed.clear();
        everUsed.resize(currentSize, false);
        numElements = 0;

        for (int i = 0; i < oldSize; ++i) {
            if (oldOccupied[i]) {
                insert(oldTable[i]);
            }
        }
    }

    int findPos(int key) {
        int offset = 1;
        int index = hash(key);

        while (everUsed[index] && (occupied[index] && table[index] != key)) {
            index = (index + offset * offset) % currentSize; // Quadratic probing
            offset++;
            if (offset > currentSize) { // Prevent infinite loops
                return -1;
            }
        }
        return index;
    }

public:
    HashTable(int size) : currentSize(nextPrime(size)), numElements(0) {
        table.resize(currentSize, INT_MIN);
        occupied.resize(currentSize, false);
        everUsed.resize(currentSize, false);
    }

    void insert(int key) {
        if (float(numElements + 1) / currentSize >= 0.8) {
            resize();
        }
        int pos = findPos(key);
        if (pos == -1) {
            std::cout << "Max probing limit reached!" << std::endl;
            return;
        }
        if (occupied[pos]) {
            std::cout << "Duplicate key insertion is not allowed" << std::endl;
            return;
        }
        table[pos] = key;
        occupied[pos] = true;
        everUsed[pos] = true;
        numElements++;
    }

    void remove(int key) {
        int pos = findPos(key);
        if (!occupied[pos]) {
            std::cout << "Element not found" << std::endl;
            return;
        }
        occupied[pos] = false;
        numElements--;
    }

    int search(int key) {
        int pos = findPos(key);
        return occupied[pos] ? pos : -1;
    }

    void printTable() {
        for (int i = 0; i < currentSize; ++i) {
            if (occupied[i])
                std::cout << table[i] << " ";
            else
                std::cout << "- ";
        }
        std::cout << std::endl;
    }
};
