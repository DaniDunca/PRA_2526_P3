
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <stdexcept>
#include <ostream>
#include "ListLinked.h"

// Entrada (par clave-valor)
template <typename V>
struct TableEntry {
    std::string key;
    V value;
    TableEntry() = default;
    TableEntry(const std::string& k, const V& v) : key(k), value(v) {}
};

template <typename V>
class HashTable {
private:
    int capacity_;
    int entries_;
    ListLinked<TableEntry<V>>* buckets_;

    int hash(const std::string& key) const {
        unsigned sum = 0;
        for (unsigned char c : key) sum += c;      // suma ASCII
        return static_cast<int>(sum % capacity_);
    }

    int indexOf(int bucket, const std::string& key) const {
        const auto& L = buckets_[bucket];
        for (int i = 0; i < L.size(); ++i) {
            if (L.get(i).key == key) return i;
        }
        return -1;
    }

public:
    explicit HashTable(int cap = 3)
        : capacity_(cap), entries_(0),
          buckets_(new ListLinked<TableEntry<V>>[cap]) {}

    ~HashTable() { delete[] buckets_; }

    int capacity() const { return capacity_; }
    int entries()  const { return entries_;  }

    void insert(const std::string& key, const V& value) {
        int b = hash(key);
        if (indexOf(b, key) != -1)
            throw std::runtime_error("Key '" + key + "' already exists!");
        buckets_[b].insert(0, TableEntry<V>{key, value}); // push-front
        ++entries_;
    }

    V search(const std::string& key) const {
        int b = hash(key);
        int pos = indexOf(b, key);
        if (pos == -1)
            throw std::runtime_error("Key '" + key + "' not found!");
        return buckets_[b].get(pos).value;
    }

    // Acceso estilo diccionario (por valor; usa search)
    V operator[](const std::string& key) const { return search(key); }

    V remove(const std::string& key) {
        int b = hash(key);
        int pos = indexOf(b, key);
        if (pos == -1)
            throw std::runtime_error("Key '" + key + "' not found!");
        TableEntry<V> e = buckets_[b].remove(pos); // remove por valor
        --entries_;
        return e.value;
    }

    friend std::ostream& operator<<(std::ostream& out, const HashTable<V>& dict) {
        out << "HashTable [entries: " << dict.entries_ << ", capacity: " << dict.capacity_ << "]\n";
        out << "==============\n\n";

        for (int b = 0; b < dict.capacity_; ++b) {
            out << "\n== Cubeta " << b << " ==\n\n";
            const auto& L = dict.buckets_[b];
            out << "List => ";
            if (L.empty()) {
                out << "[]\n";
            } else {
                out << "[\n";
                for (int i = 0; i < L.size(); ++i) {
                    const auto& e = L.get(i);
                    out << "  ('" << e.key << "' => " << e.value << ")\n";
                }
                out << "]\n";
            }
        }

        out << "\n==============\n";
        return out;
    }
};

#endif // HASH_TABLE_H
