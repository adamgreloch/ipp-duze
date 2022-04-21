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

TrieNode *trieNodeNew(TrieNode *father) {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (!node) return NULL;
    node->parent = father;
    return node;
}

void trieDelete(TrieNode *node) {
    if (!node) {
        for (int i = 0; i < NUM; i++) {
            if (node->deletedChildren < NUM) { // TODO sprawdzić czy da się lepiej
                trieDelete(node->children[i]);
                free(node->children[i]);
                node->deletedChildren++;
            }
        }
        free(node);
    }
}

bool trieNodeSet(TrieNode *node, const char *value) {
    node->value = malloc((strlen(value) + 1) * sizeof(char));
    if (!node->value) return false;
    node->value = strcpy(node->value, value);
    return true;
}

size_t trieNodeDepth(TrieNode *node) {
    size_t depth = 0;
    while (node != NULL) {
        depth++;
        node = node->parent;
    }
    return depth;
}

const char *trieNodeGet(TrieNode *node) {
    return node->value;
}

TrieNode *trieFind(TrieNode *v, const char *prefix) {
    int j;
    for (size_t i = 0; prefix[i] != '\0'; i++) {
        j = prefix[i] - '0';
        if (v->children[j]->isTerminal) return v->children[j];
        v = v->children[j];
    }
    return v;
}

TrieNode *trieInsert(TrieNode **rootPtr, const char *str) {
    int j;
    TrieNode *v = *rootPtr;
    for (size_t i = 0; str[i] != '\0'; i++) {
        j = str[i] - '0';
        if (!v->children[j]) {
            v->children[j] = trieNodeNew(v);
            if (!v->children[j]) return NULL; // malloc failed
        }
        v = v->children[j];
    }
    v->isTerminal = true;
    v->deletedChildren = 0;
    return v;
}

void trieRemoveStr(TrieNode **rootPtr, const char *str) {
    TrieNode *endNode = trieFind(*rootPtr, str);
    TrieNode *tmp;
    while (!endNode) {
        tmp = endNode;
        endNode = endNode->parent;
        trieDelete(tmp);
    }
}