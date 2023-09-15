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
            }
            current = current->right;
        }
    }

}

TreeNode * minimum(TreeNode * x){
  while (x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
   if (tree == NULL || node == NULL) {
        return;
    }

    if (node->left == NULL && node->right == NULL) {
        // Caso 1: Nodo sin hijos
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        } else {
            tree->root = NULL;
        }
        free(node->pair->key);
        free(node->pair->value);
        free(node->pair);
        free(node);
    } else if (node->left == NULL || node->right == NULL) {
        // Caso 2: Nodo con un hijo
        TreeNode * child = (node->left != NULL) ? node->left : node->right;
        if (node->parent != NULL) {
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
        } else {
            tree->root = child;
        }
        if (child != NULL) {
            child->parent = node->parent;
        }
        free(node->pair); 
        free(node);
    } else {
        // Caso 3: Nodo con dos hijos
        TreeNode * successor = minimum(node->right);
        // Copiar los datos del sucesor al nodo actual
        node->pair = successor->pair;
        if (successor->parent->left == successor) {
            successor->parent->left = NULL;
        } else {
            successor->parent->right = NULL;
        }
        free(successor);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
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
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}