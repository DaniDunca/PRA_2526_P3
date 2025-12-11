
#ifndef BSTREEDICT_H
#define BSTREEDICT_H

#include <ostream>
#include <stdexcept>
#include <string>

#include "Dict.h"
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
    ~BSTreeDict() override {
        delete tree;
        tree = nullptr;
    }

    // Número de entradas (delegado al ABB)
    int entries() const override {
        return tree->size();
    }

    // Inserta o actualiza el valor asociado a 'key'
    void insert(const std::string& key, const V& value) override {
        TableEntry<V> entry(key, value);

        // Si existe la clave, eliminamos y reinsertamos con el nuevo valor
        // (BSTree::search devuelve copia, no podemos modificar in situ).
        try {
            (void)tree->search(makeProbe(key)); // prueba de existencia
            tree->remove(makeProbe(key));       // eliminación por clave
            // no es necesario gestionar contador: BSTree lo hace internamente
        } catch (const std::runtime_error&) {
            // no existe -> seguimos a la inserción directa
        }

        tree->insert(entry);
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
        // Necesitamos el valor antes de eliminar
        TableEntry<V> e;
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

