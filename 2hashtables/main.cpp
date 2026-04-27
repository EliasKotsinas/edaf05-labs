#include <iostream>
#include <string>
#include <stddef.h>
#include <stdint.h>
#include <optional>

class HashTable {
    private:
        std::optional<std::string>* keys;
        std::optional<int>* values;
        int size;
        std::string deletedMarker = "<DELETED>";
    public:
        HashTable(int size) : size(size), keys(new std::optional<std::string>[size]), values(new std::optional<int>[size]) {}

    ~HashTable() {
        delete[] keys;
        delete[] values;
    }
    size_t hashpjw(const std::string& str)
    {
        uint32_t h = 0;
        uint32_t g;
        for (char c : str) {
            h = (h << 4) + c;
            if ((g = h & 0xf0000000)) {
                h ^= g >> 24;
                h ^= g;
            }
        }
        return h;
    }

    bool contains(std::string key) {
        size_t index = hashpjw(key) % size;
        for (int i = 0; i < size; i++) {
            size_t probeIndex = (index + i) % size;
            if (!keys[probeIndex].has_value() || keys[probeIndex] == deletedMarker) {
                return false;
            }
            if (keys[probeIndex] == key) {
                return true;
            }
        }
        return false;
    }

    void insert(std::string key, int value) {
        size_t index = hashpjw(key) % size;
        for (int i = 0; i < size; i++) {
            size_t probeIndex = (index + i) % size;
            if (!keys[probeIndex].has_value() || keys[probeIndex] == deletedMarker) {
                keys[probeIndex] = key;
                values[probeIndex] = value;
                return;
            }
            if (keys[probeIndex] == key) {
                values[probeIndex] = value;
                return;
            }
        }
        resize();
        insert(key, value);
    }

    void resize() {
        int newSize = size * 2;
        std::optional<std::string>* newKeys = new std::optional<std::string>[newSize];
        std::optional<int>* newValues = new std::optional<int>[newSize];
        for (int i = 0; i < size; i++) {
            if (keys[i].has_value() && keys[i] != deletedMarker) {
                size_t newIndex = hashpjw(keys[i].value()) % newSize;
                for (int j = 0; j < newSize; j++) {
                    size_t probeIndex = (newIndex + j) % newSize;
                    if (!newKeys[probeIndex].has_value()) {
                        newKeys[probeIndex] = keys[i];
                        newValues[probeIndex] = values[i];
                        break;
                    }
                }
            }
        }
        keys = std::move(newKeys);
        values = std::move(newValues);
        size = newSize;
    }
 

    void remove(std::string key) {
        size_t index = hashpjw(key) % size;
        for (int i = 0; i < size; i++) {
            size_t probeIndex = (index + i) % size;
            if (!keys[probeIndex].has_value()) {
                return;
            }
            if (keys[probeIndex] == key) {
                keys[probeIndex] = deletedMarker;
                values[probeIndex] = std::nullopt;

                // Rehash subsequent keys in the cluster
                size_t nextIndex = (probeIndex + 1) % size;
                while (keys[nextIndex].has_value() && keys[nextIndex] != deletedMarker) {
                    std::string rehashKey = keys[nextIndex].value();
                    int rehashValue = values[nextIndex].value();
                    keys[nextIndex] = std::nullopt;
                    values[nextIndex] = std::nullopt;
                    insert(rehashKey, rehashValue);
                    nextIndex = (nextIndex + 1) % size;
                }
                return;
            }
        }
    }

    std::pair<int, std::string> get_max() {
        int max_value = -1;
        std::string max_key;
        for (int i = 0; i < size; ++i) {
            if (keys[i].has_value() && keys[i] != deletedMarker && values[i].has_value()) {
                if (values[i].value() > max_value ||
                    (values[i].value() == max_value && keys[i].value() < max_key)) {
                    max_value = values[i].value();
                    max_key = keys[i].value();
                }
            }
        }
        return {max_value, max_key};
    }

    int get(std::string key) {
        size_t index = hashpjw(key) % size;
        for (int i = 0; i < size; i++) {
            size_t probeIndex = (index + i) % size;
            if (!keys[probeIndex].has_value() || keys[probeIndex] == deletedMarker) {
                return -1;
            }
            if (keys[probeIndex] == key) {
                return values[probeIndex].value();
            }
        }
        return -1;
    }
};

int main() {
    HashTable ht(16);
    std::string word;
    int i = 0;

    while (std::cin >> word) {
        bool is_present = ht.contains(word);
        bool remove_it = i % 16 == 0;

        if (is_present) {
            if (remove_it) {
                ht.remove(word);
            } else {
                int count = ht.get(word);
                ht.insert(word, count + 1);
            }
        } else if (!remove_it) {
            ht.insert(word, 1);
        }
        i++;
    }

    std::pair<int, std::string> max_pair = ht.get_max();
    std::cout << max_pair.second << " " << max_pair.first << std::endl;

    return 0;
}
