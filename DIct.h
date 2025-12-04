#ifndef DICT_H
#define DICT_H

#include <string>

template <typename V>
class Dict {
public:
   
    virtual void insert(const std::string& key, const V& value) = 0;

        virtual V search(const std::string& key) const = 0;

        virtual V remove(const std::string& key) = 0;

    
    virtual int entries() const = 0;
};

#endif

