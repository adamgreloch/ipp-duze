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
    for (int i = 0; i < ALLNUM; i++)
        node->children[i] = NULL;
    node->isTerminal = true;
    node->value = NULL;
    node->lastVisited = -1;

    return node;
}

/**
 * Zwalnia bezpośrednio dostępne struktury wskaźnikowe z poziomu @p node.
 * @param node
 */
static void freeNode(TrieNode *node) {
    free(node->value);
    free(node);
}

void trieDelete(TrieNode *node) {
    if (!node) return;

    int i;
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
            i = curr->lastVisited + 1;
            if (curr->children[i]) {
                curr->lastVisited = i;
                curr = curr->children[i];
            }
            else
                curr->lastVisited++;
        }
}

bool trieNodeSet(TrieNode *node, const char *value) {
    if (!node) return false;

    node->value = realloc(node->value, (strlen(value) + 1) * sizeof(char));
    if (!node->value) return false;

    strcpy(node->value, value);
    return true;
}

const char *trieNodeGet(TrieNode *node) {
    if (!node) return NULL;
    return node->value;
}

TrieNode *trieFind(TrieNode *v, const char *str, size_t *length) {
    if (!v) return NULL;
    int j;
    TrieNode *lastValueNode = NULL;

    *length = 0;
    size_t distance = 0;
    bool leaf = false;
    for (size_t i = 0; !leaf && str[i] != '\0'; i++) {
        j = str[i] - '0';
        if (!v->children[j]) leaf = true;
        else {
            v = v->children[j];
            distance++;
        }
        if (v->value) {
            lastValueNode = v;
            *length += distance;
            distance = 0;
        }
    }
    return lastValueNode;
}

TrieNode *trieInsertStr(TrieNode **rootPtr, const char *str) {
    if (!(*rootPtr)) *rootPtr = trieNodeNew(NULL);
    if (!(*rootPtr)) return NULL;

    int j;
    TrieNode *v = *rootPtr;

    for (size_t i = 0; str[i] != '\0'; i++) {
        j = str[i] - '0';
        if (!v->children[j]) {
            v->children[j] = trieNodeNew(v);
            if (!v->children[j]) return NULL;
            v->isTerminal = false;
            /* Nastąpiła zmiana struktury drzewa, więc resetujemy jeszcze stan
             * odwiedzenia tego węzła przez trieDelete(): */
            v->lastVisited = -1;
        }
        v = v->children[j];
    }
    return v;
}

void trieRemoveStr(TrieNode **rootPtr, const char *str) {
    if (*rootPtr) {
        TrieNode *v = *rootPtr;
        int j = 0;
        bool mayExist = true;
        for (size_t i = 0; mayExist && str[i] != '\0'; i++) {
            j = str[i] - '0';
            if (!v || !v->children[j]) mayExist = false;
            else
                v = v->children[j];
        }
        if (mayExist) {
            v->parent->children[j] = NULL;
            trieDelete(v);
            v = NULL;
        }
    }
}
