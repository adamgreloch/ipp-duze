/** @file
 * Implementacja klasy obsługującej strukturę danych trie
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <string.h>
#include "trie.h"
#include "linkedList.h"

struct TrieNode {
    struct TrieNode *parent; /**< Wskaźnik na rodzica węzła. */
    struct TrieNode *children[ALLNUM]; /**< Tablica wskaźników na dzieci węzła. */
    bool isTerminal; /**< Wartość logiczna przyjmująca TRUE, jeśli węzeł
                          jest liściem, FALSE w przeciwnym wypadku. */
    /**
     * Wartość węzła, która, jeśli niepusta (ma wartość inną niż NULL), to jest
     * poprawnym ciągiem znaków lub listą poprawnych ciągów znaków.
     */
    union {
        char *seq; /**< Poprawny ciąg znaków. */
        List *list; /**< Wskaźnik na listę poprawnych ciągów znaków. */
    } value;

    bool hasList; /**< [TYMCZASOWE] wartość logiczna przyjmująca TRUE, jeśli
                        węzeł zawiera listę w @p value, FALSE jeśli zawiera
                        poprawny ciąg znaków. */
    int lastVisited; /**< Ostatnio odwiedzony przez trieDelete() numer
                          dziecka. Resetowany do -1 przy zmianie
                          struktury poddrzew. */
    ListNode *bound; /**< Wskaźnik na element listy, znajdujący
                          się w @p list pewnego węzła, zawierający ciąg
                          znaków, który reprezentuje ten węzeł. */
};

TrieNode *trieNodeNew(TrieNode *parent, bool hasList) {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (!node) return NULL;

    node->parent = parent;
    for (int idx = 0; idx < ALLNUM; idx++)
        node->children[idx] = NULL;
    node->isTerminal = true;
    node->lastVisited = -1;

    node->value.seq = NULL;
    node->hasList = hasList;

    node->bound = NULL;

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
static void freeTrieNode(TrieNode *node) {
    removeListNode(node->bound);

    if (node->hasList) {
        listDelete(node->value.list);
        node->value.list = NULL;
    }
    else {
        free(node->value.seq);
        node->value.seq = NULL;
    }

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
                freeTrieNode(curr);
                curr = NULL;
            }
            else {
                toFree = curr;
                curr = curr->parent;
                /* Przekazany do zwolnienia curr był dzieckiem nowego curr.
                 * Musimy więc usunąć skojarzenie między nimi. */
                curr->children[curr->lastVisited] = NULL;
                freeTrieNode(toFree);
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

ListNode *trieAddToList(TrieNode *node, const char *value, size_t length) {
    if (!node->hasList) return NULL;

    if (!node->value.list) {
        if (!(node->value.list = listInit(value, length))) return NULL;
        else return listPollNode(node->value.list);
    }

    return listAdd(node->value.list, value, length);
}

bool trieNodeBind(TrieNode *trieNode, ListNode *listNode) {
    if (!listNode || !trieNode || trieNode->hasList) return false;
    trieNode->bound = listNode;
    return true;
}

bool trieNodeSetSeq(TrieNode *node, const char *value, size_t length) {
    if (!node) return false;

    node->value.seq = realloc(node->value.seq, (length + 1) * sizeof(char));
    if (!node->value.seq) return false;

    strcpy(node->value.seq, value);
    return true;
}

List *trieNodeGetList(TrieNode *node) {
    if (!node) return NULL;
    return node->value.list;
}

const char *trieNodeGetSeq(TrieNode *node) {
    if (!node) return NULL;
    return node->value.seq;
}

TrieNode *trieFindSeq(TrieNode *root, const char *str, size_t *length) {
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
        if (root->value.seq) {
            lastWithValue = root;
            *length += distance;
            distance = 0;
        }
    }
    return lastWithValue;
}

TrieNode *trieInsertStr(TrieNode **rootPtr, const char *str, bool hasList) {
    if (!(*rootPtr)) *rootPtr = trieNodeNew(NULL, hasList);
    if (!(*rootPtr)) return NULL;

    int idx;
    TrieNode *v = *rootPtr;

    for (size_t i = 0; str[i] != '\0'; i++) {
        idx = getIndex(str[i]);
        if (!v->children[idx]) {
            v->children[idx] = trieNodeNew(v, hasList);
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
