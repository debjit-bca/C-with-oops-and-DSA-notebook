#ifndef HASHMAP_H
#define HASHMAP_H

#include "User.h"
#include <vector>
#include <list>
#include <string>

template<typename K, typename V>
class HashNode {
public:
    K key;
    V value;
    HashNode(K k, V v) : key(k), value(v) {}
};

template<typename K, typename V>
class HashMap {
private:
    static const int TABLE_SIZE = 100;
    std::vector<std::list<HashNode<K, V>>> table;
    
    int hashFunction(const K& key) {
        std::hash<K> hasher;
        return hasher(key) % TABLE_SIZE;
    }

public:
    HashMap() : table(TABLE_SIZE) {}
    
    void insert(const K& key, const V& value) {
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        
        for (auto& node : cell) {
            if (node.key == key) {
                node.value = value;
                return;
            }
        }
        cell.push_back(HashNode<K, V>(key, value));
    }
    
    bool find(const K& key, V& value) {
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        
        for (auto& node : cell) {
            if (node.key == key) {
                value = node.value;
                return true;
            }
        }
        return false;
    }
    
    bool remove(const K& key) {
        int hashValue = hashFunction(key);
        auto& cell = table[hashValue];
        
        auto it = cell.begin();
        while (it != cell.end()) {
            if (it->key == key) {
                cell.erase(it);
                return true;
            }
            ++it;
        }
        return false;
    }
    
    bool exists(const K& key) {
        V temp;
        return find(key, temp);
    }
};

#endif
