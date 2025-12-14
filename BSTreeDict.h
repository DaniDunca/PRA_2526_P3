
#ifndef BSTREEDICT_H
#define BSTREEDICT_H

#include <ostream>
#include <stdexcept>
#include <string>

#include "DIct.h"      // mantenemos el nombre tal cual en tu repo
#include "BSTree.h"
#include "TableEntry.h"

/**
 * Diccionario basado en ABB (BST) que almacena TableEntry<V> ordenados por clave.
 * Delegamos en BSTree<TableEntry<V>> todas las operaciones.
 */
template <typename V>
class BSTreeDict : public Dict<V> {

private:
    // ABB con elementos TableEntry<V> (clave->valor)
    BSTree<TableEntry<V>>* tree;

    // Crea una entrada "sonda" que solo contiene la clave (para buscar/eliminar)
    static TableEntry<V> makeProbe(const std::string& key) {
        return TableEntry<V>(key);
    }

public:
    // Constructor: ABB vacío con memoria dinámica
    BSTreeDict()
        : tree(new BSTree<TableEntry<V>>()) {}

    // Destructor: libera el ABB
    ~BSTreeDict() {
        delete tree;
        tree = nullptr;
    }

    // Número de entradas (delegado al ABB)
    int entries() const override {
        return tree->size();
    }

    // Inserta (key, value).
    // Si la clave ya existe, debe lanzar std::runtime_error (el test lo exige).
    void insert(const std::string& key, const V& value) override {
        // Si search no lanza, la clave existe -> lanzar.
        try {
            (void) tree->search(makeProbe(key));
            throw std::runtime_error("Clave duplicada: " + key);
        } catch (const std::runtime_error&) {
            // No existe: insertamos.
        }
        tree->insert(TableEntry<V>(key, value));
    }

    // Busca 'key' y devuelve el valor; lanza si no existe
    V search(const std::string& key) const override {
        try {
            TableEntry<V> e = tree->search(makeProbe(key));
            return e.getValue();
        } catch (const std::runtime_error&) {
            throw std::runtime_error("Clave no encontrada: " + key);
        }
    }

    // Elimina la entrada con 'key' y devuelve el valor eliminado; lanza si no existe
    V remove(const std::string& key) override {
        TableEntry<V> e;  // requiere ctor por defecto en TableEntry
        try {
            e = tree->search(makeProbe(key));
        } catch (const std::runtime_error&) {
            throw std::runtime_error("No se puede eliminar: clave no encontrada: " + key);
        }
        tree->remove(makeProbe(key));
        return e.getValue();
    }

    // Operador [] como interfaz a search(key)
    V operator[](const std::string& key) const {
        return this->search(key);
    }

    // Impresión del contenido del diccionario (inorden del ABB)
    friend std::ostream& operator<<(std::ostream& out, const BSTreeDict<V>& bs) {
        if (bs.tree != nullptr) {
            out << *(bs.tree); // delega en BSTree<TableEntry<V>>::operator<<
        }
        return out;
    }
};

#endif
