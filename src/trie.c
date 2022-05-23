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
    for (int idx = 0; idx < ALLNUM; idx++)
        node->children[idx] = NULL;
    node->isTerminal = true;
    node->seq = NULL;
    node->lastVisited = -1;

    return node;
}

static int getIndex(char c) {
    switch (c) {
        case '*':
            return 10;
        case '#':
            return 11;
        default:
            return c - '0';
    }
}

/**
 * Zwalnia wartość węzła @p node oraz sam węzeł.
 * @param node - wskaźnik na węzeł.
 */
static void freeNode(TrieNode *node) {
    free(node->seq);
    free(node);
}

void trieDelete(TrieNode *node) {
    if (!node) return;

    int idx;
    TrieNode *toFree;
    TrieNode *curr = node;

    while (curr)
        if (curr->isTerminal || curr->lastVisited == ALLNUM - 1) {
            /* Jeśli napotkany węzeł nie ma dzieci, to zwalniamy go. */
            if (curr == node) {
                freeNode(curr);
                curr = NULL;
            }
            else {
                toFree = curr;
                curr = curr->parent;
                /* Przekazany do zwolnienia curr był dzieckiem nowego curr.
                 * Musimy więc usunąć skojarzenie między nimi. */
                curr->children[curr->lastVisited] = NULL;
                freeNode(toFree);
            }
        }
        else {
            /* W przeciwnym razie szukamy węzłów bez dzieci do usunięcia. */
            idx = curr->lastVisited + 1;
            if (curr->children[idx]) {
                curr->lastVisited = idx;
                curr = curr->children[idx];
            }
            else
                curr->lastVisited++;
        }
}

bool trieNodeSet(TrieNode *node, const char *value, size_t length) {
    if (!node) return false;

    node->seq = realloc(node->seq, (length + 1) * sizeof(char));
    if (!node->seq) return false;

    strcpy(node->seq, value);
    return true;
}

const char *trieNodeGet(TrieNode *node) {
    if (!node) return NULL;
    return node->seq;
}

TrieNode *trieFind(TrieNode *root, const char *str, size_t *length) {
    if (!root) return NULL;
    int idx;
    TrieNode *lastWithValue = NULL;

    *length = 0;
    size_t distance = 0;
    bool leaf = false;
    for (size_t i = 0; !leaf && str[i] != '\0'; i++) {
        idx = getIndex(str[i]);
        if (!root->children[idx]) leaf = true;
        else {
            root = root->children[idx];
            distance++;
        }
        if (root->seq) {
            lastWithValue = root;
            *length += distance;
            distance = 0;
        }
    }
    return lastWithValue;
}

TrieNode *trieInsertStr(TrieNode **rootPtr, const char *str) {
    if (!(*rootPtr)) *rootPtr = trieNodeNew(NULL);
    if (!(*rootPtr)) return NULL;

    int idx;
    TrieNode *v = *rootPtr;

    for (size_t i = 0; str[i] != '\0'; i++) {
        idx = getIndex(str[i]);
        if (!v->children[idx]) {
            v->children[idx] = trieNodeNew(v);
            if (!v->children[idx]) return NULL;
            v->isTerminal = false;
            /* Nastąpiła zmiana struktury drzewa, więc resetujemy jeszcze stan
             * odwiedzenia tego węzła: */
            v->lastVisited = -1;
        }
        v = v->children[idx];
    }
    return v;
}

void trieRemoveStr(TrieNode **rootPtr, const char *str) {
    if (*rootPtr) {
        TrieNode *v = *rootPtr;
        int idx = 0;
        bool mayExist = true;
        for (size_t i = 0; mayExist && str[i] != '\0'; i++) {
            idx = getIndex(str[i]);
            if (!v || !v->children[idx]) mayExist = false;
            else
                v = v->children[idx];
        }
        if (mayExist) {
            v->parent->children[idx] = NULL;
            trieDelete(v);
            v = NULL;
        }
    }
}
