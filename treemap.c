#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *)malloc(sizeof(TreeMap));
    if (map == NULL) {
        return NULL;
    }
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || searchTreeMap(tree, key) != NULL) {
        return; // No permitir claves duplicadas
    }

    TreeNode * new_node = createTreeNode(key, value);
    if (new_node == NULL) {
        return;
    }

    if (tree->root == NULL) {
        tree->root = new_node;
        tree->current = new_node;
        return;
    }

    TreeNode * current = tree->root;
    while (1) {
        if (tree->lower_than(key, current->pair->key)) {
            if (current->left == NULL) {
                current->left = new_node;
                new_node->parent = current;
                tree->current = new_node;
                return;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = new_node;
                new_node->parent = current;
                tree->current = new_node;
                return;
            }
            current = current->right;
        }
    }
}

TreeNode * minimum(TreeNode * x) {
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) {
        return;
    }

    // Caso 1: El nodo a eliminar es una hoja (no tiene hijos).
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            // El nodo es la raíz del árbol.
            tree->root = NULL;
        } else {
            if (node == node->parent->left) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }
        free(node->pair);
        free(node);
    }

    // Caso 2: El nodo a eliminar tiene un solo hijo.
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) {
            // El nodo es la raíz del árbol.
            tree->root = child;
            child->parent = NULL;
        } else {
            if (node == node->parent->left) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        }
        free(node->pair);
        free(node);
    }

    // Caso 3: El nodo a eliminar tiene dos hijos.
    else {
        TreeNode* successor = minimum(node->right); 
        node->pair->key = successor->pair->key;    
        node->pair->value = successor->pair->value;
        removeNode(tree, successor);               
    }
}

void eraseTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) {
        return;
    }

    if (searchTreeMap(tree, key) == NULL) {
        return;
    }
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL || key == NULL) {
        return NULL;
    }
    TreeNode * current = tree->root;
    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current; // Actualizar el puntero current
            return current->pair;
        } else if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL; // La clave no se encontró en el árbol.
}

Pair * upperBound(TreeMap * tree, void* key) {
     if (tree == NULL || tree->root == NULL || key == NULL) {
        return NULL;
    }

    TreeNode * current = tree->root;
    TreeNode * upper_bound = NULL;

    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            upper_bound = current; // Actualizar el candidato a upper_bound
            current = current->left;
        } else if (is_equal(tree, key, current->pair->key)) {
            // Se encontró una clave igual, lo consideramos como upper_bound
            tree->current = current; // Actualizar el puntero current
            return current->pair;
        } else {
            current = current->right;
        }
    }

    if (upper_bound != NULL) {
        tree->current = upper_bound; // Actualizar el puntero current
        return upper_bound->pair;
    } else {
        return NULL; // No se encontró un valor mayor o igual en el árbol.
    }
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) {
        return NULL;
    }

    TreeNode* min_node = minimum(tree->root);
    tree->current = min_node;
    return min_node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) {
        return NULL;
    }

    TreeNode* current = tree->current;
    if (current->right != NULL) {
        tree->current = minimum(current->right);
    } else {
        while (current->parent != NULL && current == current->parent->right) {
            current = current->parent;
        }
        tree->current = current->parent;
    }

    if (tree->current != NULL) {
        return tree->current->pair;
    } else {
        return NULL;
    }
}