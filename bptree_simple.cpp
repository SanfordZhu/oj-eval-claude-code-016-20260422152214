#include "bptree_simple.h"
#include <algorithm>
#include <sstream>

BPTree::BPTree(const std::string& fname) : filename(fname), dirty(false) {
    load_from_file();
}

BPTree::~BPTree() {
    if (dirty) {
        save_to_file();
    }
}

void BPTree::load_from_file() {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile.is_open()) {
        // File doesn't exist, start with empty data
        return;
    }

    // Read number of entries
    size_t num_keys;
    infile.read(reinterpret_cast<char*>(&num_keys), sizeof(num_keys));

    for (size_t i = 0; i < num_keys; i++) {
        // Read key length
        size_t key_len;
        infile.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));

        // Read key
        std::string key(key_len, '\0');
        infile.read(&key[0], key_len);

        // Read number of values
        size_t num_values;
        infile.read(reinterpret_cast<char*>(&num_values), sizeof(num_values));

        // Read values
        for (size_t j = 0; j < num_values; j++) {
            int value;
            infile.read(reinterpret_cast<char*>(&value), sizeof(value));
            data[key].insert(value);
        }
    }

    infile.close();
}

void BPTree::save_to_file() {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile.is_open()) {
        return;
    }

    // Write number of keys
    size_t num_keys = data.size();
    outfile.write(reinterpret_cast<const char*>(&num_keys), sizeof(num_keys));

    for (const auto& pair : data) {
        // Write key length
        size_t key_len = pair.first.length();
        outfile.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));

        // Write key
        outfile.write(pair.first.c_str(), key_len);

        // Write number of values
        size_t num_values = pair.second.size();
        outfile.write(reinterpret_cast<const char*>(&num_values), sizeof(num_values));

        // Write values
        for (int value : pair.second) {
            outfile.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }

    outfile.close();
    dirty = false;
}

void BPTree::insert(const std::string& key, int value) {
    data[key].insert(value);
    dirty = true;
}

void BPTree::remove(const std::string& key, int value) {
    auto it = data.find(key);
    if (it != data.end()) {
        it->second.erase(value);
        if (it->second.empty()) {
            data.erase(it);
        }
        dirty = true;
    }
}

std::vector<int> BPTree::find(const std::string& key) {
    std::vector<int> result;
    auto it = data.find(key);
    if (it != data.end()) {
        result.assign(it->second.begin(), it->second.end());
    }
    return result;
}