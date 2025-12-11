
#include <iostream>
#include "HashTable.h"

int main() {
    HashTable<int> dict(3);

    // Estado inicial
    std::cout << dict << "\n";

    std::cout << "dict.capacity(): " << dict.capacity() << "\n";
    std::cout << "dict.entries(): " << dict.entries() << "\n\n";

    // Inserciones
    std::cout << "dict.insert('One', 1) ...\n";
    dict.insert("One", 1);
    std::cout << "dict.insert('Two', 2) ...\n";
    dict.insert("Two", 2);
    std::cout << "dict.insert('Three', 3) ...\n";
    dict.insert("Three", 3);
    std::cout << "dict.insert('Four', 4) ...\n";
    dict.insert("Four", 4);
    std::cout << "dict.insert('Five', 5) ...\n";
    dict.insert("Five", 5);
    std::cout << "dict.insert('Six', 6) ...\n";
    dict.insert("Six", 6);
    std::cout << "\n\n" << dict << "\n";

    // Consultas y borrado
    std::cout << "dict.search('One'): " << dict.search("One") << "\n";
    std::cout << "dict['Four']: " << dict["Four"] << "\n";
    std::cout << "dict.remove('Three'): " << dict.remove("Three") << "\n\n\n";

    std::cout << dict << "\n";

    // Excepciones pedidas
    try { dict.insert("One", 1); }
    catch (const std::runtime_error& e) {
        std::cout << "dict.insert('One') => throwed std::runtime_error: " << e.what() << "\n";
    }
    try { dict.search("Ten"); }
    catch (const std::runtime_error& e) {
        std::cout << "dict.search('Ten') => throwed std::runtime_error: " << e.what() << "\n";
    }
    try { dict.remove("Ten"); }
    catch (const std::runtime_error& e) {
        std::cout << "dict.remove('Ten') => throwed std::runtime_error: " << e.what() << "\n";
    }

    return 0;
}

