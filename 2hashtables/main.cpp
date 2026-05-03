#include <iostream>
#include <string>
#include <unordered_map>
#include <stddef.h>
#include <stdint.h>
#include <optional>

enum class State
{
    EMPTY,
    OCCUPIED,
    TOMBSTONE
};

struct HashEntry
{
    std::string key;
    int value;
    State state = State::EMPTY;
};

class HashTable
{
private:
    HashEntry *table;
    size_t capacity;
    size_t num_elements;

    size_t hash(const std::string &key)
    {
        size_t h = 0;
        for (char c : key)
        {
            h = 31 * h + c;
        }
        return h % capacity;
    }

    void rehash()
    {
        size_t old_capacity = capacity;
        HashEntry *old_table = table;

        capacity = capacity * 2;
        table = new HashEntry[capacity];

        num_elements = 0;

        for (size_t i = 0; i < old_capacity; i++)
        {
            if (old_table[i].state == State::OCCUPIED)
            {
                size_t index = hash(old_table[i].key);

                while (table[index].state != State::EMPTY)
                {
                    index = (index + 1) % capacity;
                }

                table[index].key = old_table[i].key;
                table[index].value = old_table[i].value;
                table[index].state = State::OCCUPIED;
                num_elements++;
            }
        }

        delete[] old_table;
    }

public:
    HashTable(size_t initial_capacity = 32)
    {
        capacity = initial_capacity;
        num_elements = 0;
        table = new HashEntry[capacity];
    }

    ~HashTable()
    {
        delete[] table;
    }

    void insert(const std::string &word)
    {
        float load_factor = (float)num_elements / capacity;
        if (load_factor >= 0.5f)
        {
            rehash();
        }

        size_t index = hash(word);
        int first_tombstone = -1;

        while (table[index].state != State::EMPTY)
        {
            if (table[index].state == State::OCCUPIED)
            {
                if (table[index].key == word)
                {
                    table[index].value++;
                    return;
                }
            }
            else if (table[index].state == State::TOMBSTONE)
            {
                if (first_tombstone == -1)
                {
                    first_tombstone = index;
                }
            }
            index = (index + 1) % capacity;
        }

        size_t insert_index = (first_tombstone != -1) ? first_tombstone : index;

        table[insert_index].key = word;
        table[insert_index].value = 1;
        table[insert_index].state = State::OCCUPIED;

        if (first_tombstone == -1)
        {
            num_elements++;
        }
    }

    void remove(const std::string &word)
    {
        size_t index = hash(word);

        while (table[index].state != State::EMPTY)
        {
            if (table[index].state == State::OCCUPIED && table[index].key == word)
            {
                table[index].state = State::TOMBSTONE;
                return;
            }
            index = (index + 1) % capacity;
        }
    }

    bool contains(const std::string &word)
    {
        size_t index = hash(word);

        while (table[index].state != State::EMPTY)
        {
            if (table[index].state == State::OCCUPIED && table[index].key == word)
            {
                return true;
            }
            index = (index + 1) % capacity;
        }
        return false;
    }

    void print_most_frequent()
    {
        int max_count = -1;

        std::string best_word = "";

        for (size_t i = 0; i < capacity; i++)
        {
            if (table[i].state == State::OCCUPIED)
            {
                int count = table[i].value;
                std::string current_word = table[i].key;

                if (count > max_count)
                {
                    max_count = count;
                    best_word = current_word;
                }
                else if (count == max_count && current_word < best_word)
                {
                    best_word = current_word;
                }
            }
        }

        std::cout << best_word << " " << max_count << "\n";
    }
};

int main()
{
    HashTable d;
    int i = 0;
    std::string line;

    while (std::getline(std::cin, line))
    {
        size_t start = line.find_first_not_of(" \t\r\n");
        std::string word = "";
        if (start != std::string::npos)
        {
            size_t end = line.find_last_not_of(" \t\r\n");
            word = line.substr(start, end - start + 1);
        }

        bool is_present = d.contains(word);
        bool remove_it = (i % 16 == 0);

        if (is_present)
        {
            if (remove_it)
            {
                d.remove(word);
            }
            else
            {
                d.insert(word);
            }
        }
        else if (!remove_it)
        {
            d.insert(word);
        }
        i++;
    }

    d.print_most_frequent();

    return 0;
}
