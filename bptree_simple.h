#ifndef BPTREE_SIMPLE_H
#define BPTREE_SIMPLE_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <iostream>

class BPTree {
private:
    std::map<std::string, std::set<int>> data;
    std::string filename;
    bool dirty;

    void load_from_file();
    void save_to_file();

public:
    BPTree(const std::string& fname);
    ~BPTree();

    void insert(const std::string& key, int value);
    void remove(const std::string& key, int value);
    std::vector<int> find(const std::string& key);
};

#endif