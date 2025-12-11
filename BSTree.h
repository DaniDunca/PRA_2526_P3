
#ifndef BSTREE_H
#define BSTREE_H

#include <ostream>
#include <stdexcept>
#include "BSNode.h"

/**
 * Árbol Binario de Búsqueda (ABB) genérico.
 * Oculta la estructura interna (BSNode<T>) al usuario; solo expone operaciones de alto nivel.
 */
template <typename T>
class BSTree {
private:
    int nelem = 0;         // número de elementos almacenados
    BSNode<T>* root = nullptr; // nodo raíz del ABB

private:
    // --- BÚSQUEDA (recursiva) ---
    BSNode<T>* search(BSNode<T>* n, T e) const {
        if (n == nullptr) {
            throw std::runtime_error("Element not found");
        } else if (n->elem < e) {
            return search(n->right, e);
        } else if (n->elem > e) {
            return search(n->left, e);
        } else {
            return n; // encontrado
        }
    }

    // --- INSERCIÓN (recursiva) ---
    BSNode<T>* insert(BSNode<T>* n, T e) {
        if (n == nullptr) {
            ++nelem;
            return new BSNode<T>(e);
        } else if (n->elem == e) {
            throw std::runtime_error("Duplicated element");
        } else if (n->elem < e) {
            n->right = insert(n->right, e);
        } else {
            n->left = insert(n->left, e);
        }
        return n;
    }

    // --- IMPRESIÓN (inorden) ---
    void print_inorder(std::ostream &out, BSNode<T>* n) const {
        if (n == nullptr) return;
        print_inorder(out, n->left);
        out << n->elem << '\n';
        print_inorder(out, n->right);
    }

    // --- ELIMINACIÓN (recursiva) ---
    BSNode<T>* remove(BSNode<T>* n, T e) {
        if (n == nullptr) {
            throw std::runtime_error("Element not found");
        }

        if (e < n->elem) {
            n->left = remove(n->left, e);
        } else if (e > n->elem) {
            n->right = remove(n->right, e);
        } else {
            // n es el nodo a eliminar
            if (n->left == nullptr && n->right == nullptr) {
                // caso 1: sin hijos
                delete n;
                --nelem;
                return nullptr;
            } else if (n->left == nullptr) {
                // caso 2: solo hijo derecho
                BSNode<T>* r = n->right;
                delete n;
                --nelem;
                return r;
            } else if (n->right == nullptr) {
                // caso 3: solo hijo izquierdo
                BSNode<T>* l = n->left;
                delete n;
                --nelem;
                return l;
            } else {
                // caso 4: dos hijos
                // política: reemplazar por el máximo del subárbol izquierdo
                T m = max(n->left);
                n->elem = m;
                n->left = remove_max(n->left); // remove_max decrementa nelem
            }
        }
        return n;
    }

    // Devuelve el máximo elemento del subárbol con raíz n
    T max(BSNode<T>* n) const {
        if (n == nullptr) {
            throw std::runtime_error("Element not found");
        }
        while (n->right != nullptr) n = n->right;
        return n->elem;
    }

    // Elimina el máximo elemento del subárbol con raíz n y devuelve la nueva raíz
    BSNode<T>* remove_max(BSNode<T>* n) {
        if (n == nullptr) {
            throw std::runtime_error("Element not found");
        }
        // el máximo está más a la derecha
        if (n->right == nullptr) {
            // n es el máximo; reemplazar n por su hijo izquierdo
            BSNode<T>* l = n->left;
            delete n;
            --nelem; // eliminamos físicamente un nodo
            return l;
        } else {
            n->right = remove_max(n->right);
            return n;
        }
    }

    // --- DESTRUCCIÓN (recursiva) ---
    void delete_cascade(BSNode<T>* n) {
        if (n == nullptr) return;
        delete_cascade(n->left);
        delete_cascade(n->right);
        delete n;
    }

public:
    // --- CREACIÓN Y TAMAÑO ---
    BSTree() = default;

    ~BSTree() {
        delete_cascade(root);
        root = nullptr;
        nelem = 0;
    }

    int size() const { return nelem; }

    // --- BÚSQUEDA (lanzadera) ---
    T search(T e) const {
        BSNode<T>* n = search(root, e);
        return n->elem; // devolver el elemento del nodo encontrado
    }

    // operator[] como interfaz de búsqueda
    T operator[](T e) const { return search(e); }

    // --- INSERCIÓN (lanzadera) ---
    void insert(T e) {
        root = insert(root, e);
    }

    // --- ELIMINACIÓN (lanzadera) ---
    void remove(T e) {
        root = remove(root, e);
    }

    // --- IMPRESIÓN ---
    friend std::ostream& operator<<(std::ostream &out, const BSTree<T> &bst) {
        bst.print_inorder(out, bst.root); // recorrido inorden (menor -> mayor)
        return out;
    }
};

#endif // BSTREE_H

