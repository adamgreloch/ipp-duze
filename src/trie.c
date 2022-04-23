/** @file
 * Implementacja klasy obsługującej strukturę danych trie
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <malloc.h>
#include <string.h>
#include "trie.h"

TrieNode *trieNodeNew(TrieNode *parent) {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (!node) return NULL;

    node->parent = parent;
    node->children = calloc(ALLNUM, sizeof(TrieNode*));
    node->isTerminal = true;
    node->value = NULL;

    return node;
}

void trieDelete(TrieNode *node) {
    if (!node) return;

    for (int i = 0; i < ALLNUM; i++) {
        trieDelete(node->children[i]);
        node->children[i] = NULL;
    }

    free(node->children);
    free(node->value);
    free(node);
}

bool trieNodeSet(TrieNode *node, const char *value) {
    if (!node) return false;

    node->value = malloc((strlen(value) + 1) * sizeof(char));
    if (!node->value) return false;

    strcpy(node->value, value); // TODO sprawdzić czy tak jest poprawnie
    return node->value;
}

const char *trieNodeGet(TrieNode *node) {
    if (!node) return NULL;
    return node->value;
}

TrieNode *trieFind(TrieNode *v, const char *prefix, size_t *length) {
    if (!v) return NULL;
    int j;
    *length = 0;
    bool leaf = false;
    for (size_t i = 0; !leaf && prefix[i] != '\0'; i++) {
        j = prefix[i] - '0';
        if (!v->children[j]) leaf = true;
        else {
            v = v->children[j];
            (*length)++;
        }
    }
    while (v && !v->value) {
        v = v->parent;
        (*length)--;
    }
    return v;
}

TrieNode *trieInsert(TrieNode **rootPtr, const char *str) {
    if (!(*rootPtr))
        *rootPtr = trieNodeNew(NULL);
    int j;
    TrieNode *v = *rootPtr;

    for (size_t i = 0; str[i] != '\0'; i++) {
        j = str[i] - '0';
        if (!v->children[j]) {
            v->children[j] = trieNodeNew(v);
            if (!v->children[j]) return NULL; // malloc failed
            v->isTerminal = false;
        }
        v = v->children[j];
    }
    return v;
}

void trieRemoveStr(TrieNode **rootPtr, const char *str) {
    if (*rootPtr) {
        TrieNode *v = *rootPtr;
        int j;
        bool mayExist = true;
        for (size_t i = 0; mayExist && str[i] != '\0'; i++) {
            j = str[i] - '0';
            if (!v) mayExist = false;
            else
                v = v->children[j];
        }
        if (mayExist) {
            trieDelete(v);
            v = NULL;
        }
    }
}
