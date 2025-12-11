
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <ostream>
#include <stdexcept>
#include <string>
#include "Dict.h"
#include "TableEntry.h"
#include "ListLinked.h"

template <typename V>
class HashTable: public Dict<V> {
private:
    int n;    
    int max;  
    
    ListLinked<TableEntry<V>>** table;

    
    int h(const std::string& key) const {
        long sum = 0;
        for (unsigned char c : key) {
            sum += static_cast<int>(c);
        }
        return (max == 0) ? 0 : static_cast<int>(sum % max);
    }

    
    int findPosInBucket(int bucketIndex, const std::string& key) const {
        const auto* lst = table[bucketIndex];        
        int sz = lst->size();                      
        for (int i = 0; i < sz; ++i) {
            const TableEntry<V>& te = lst->get(i);   
            if (te.key == key) {
                return i;
            }
        }
        return -1;
    }

public:
    
    explicit HashTable(int size)
        : n(0), max(size), table(nullptr) {
        if (size <= 0) {
            throw std::runtime_error("El tamaño de la tabla debe ser positivo.");
        }
        
        table = new ListLinked<TableEntry<V>>*[max];
        for (int i = 0; i < max; ++i) {
            table[i] = new ListLinked<TableEntry<V>>();
        }
    }

    
    ~HashTable() {
        if (table) {
            for (int i = 0; i < max; ++i) {
                delete table[i];
            }
            delete[] table;
        }
    }

    
    void insert(const std::string& key, const V& value) override {
        int b = h(key);
        int pos = findPosInBucket(b, key);
        if (pos != -1) {
            throw std::runtime_error("La clave ya existe: " + key);
        }
        
        table[b]->insert(0, TableEntry<V>(key, value));
        ++n;
    }

   
    V search(const std::string& key) const override {
        int b = h(key);
        int pos = findPosInBucket(b, key);
        if (pos == -1) {
            throw std::runtime_error("Clave no encontrada: " + key);
        }
        return table[b]->get(pos).value;
    }

   
    V remove(const std::string& key) override {
        int b = h(key);
        int pos = findPosInBucket(b, key);
        if (pos == -1) {
            throw std::runtime_error("Clave no encontrada: " + key);
        }
        
        TableEntry<V> removed = table[b]->remove(pos);
        --n;
        return removed.value;
    }

  
    int entries() const override {
        return n;
    }



   
    int capacity() const {
        return max;
    }

    
    V operator[](const std::string& key) const {
        return search(key);
    }

    
    friend std::ostream& operator<<(std::ostream& out, const HashTable<V>& th) {
        out << "HashTable(capacity=" << th.max << ", entries=" << th.n << ")\n";
        for (int i = 0; i < th.max; ++i) {
            out << "[" << i << "]: ";
            int sz = th.table[i]->size();
            for (int j = 0; j < sz; ++j) {
                out << th.table[i]->get(j);  
                if (j + 1 < sz) out << " | ";
            }
            out << "\n";
        }
        return out;
    }
};

#endif 

