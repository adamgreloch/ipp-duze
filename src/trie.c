/** @file
 * Implementacja klasy obsługującej strukturę danych trie dla ciągów znaków
 * (zdefiniowanych w @ref alphabet.h). Klasa umożliwia utworzenie dwóch
 * wzajemnie zależnych drzew. Klasa implementuje strukturę @p TrieNode
 * zadeklarowaną w interfejsie @ref structs.h.
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <string.h>
#include "trie.h"
#include "linked_list.h"
#include "alphabet.h"

/**
 * Struktura przechowująca węzeł drzewa trie.
 */
struct TrieNode {
    struct TrieNode *parent; /**< Wskaźnik na rodzica węzła. */
    struct TrieNode *children[ALLNUM]; /**< Tablica wskaźników na dzieci węzła. */
    size_t count; /**< Liczba dzieci węzła. */

    /**
     * Wartość węzła, która, jeśli niepusta (ma wartość inną niż NULL), to jest
     * poprawnym ciągiem znaków lub listą poprawnych ciągów znaków.
     */
    union {
        char *seq; /**< Poprawny ciąg znaków. */
        List *list; /**< Wskaźnik na listę poprawnych ciągów znaków. */
    } value;

    bool hasList; /**< Wartość @p true, jeśli węzeł zawiera listę w @p value,
                       wartość @p false, jeśli zawiera poprawny ciąg znaków. */
    int lastVisited; /**< Ostatnio odwiedzony przez trieDelete() numer
                          dziecka. Resetowany do @p -1 przy zmianie
                          struktury poddrzew. */
    ListNode *bound; /**< Wskaźnik na element listy, znajdujący
                          się w @p list pewnego węzła, zawierający ciąg
                          znaków, który reprezentuje ten węzeł. */
    TrieNode **pointedBy; /**< Wskaźnik na wskaźnik na ten węzeł,
                               znajdujący się w tablicy rodzica. Przy usuwaniu
                               węzła służy do ustawiania ewaluowanego
                               wskaźnika na NULL, co zapobiega dostępowi do
                               zwolnionej pamięci przy przeszukiwaniu tablicy
                               dzieci rodzica. */
};

TrieNode *trieNodeNew(TrieNode *parent, bool hasList, TrieNode **pointedBy) {
    TrieNode *node = malloc(sizeof(TrieNode));
    if (!node) return NULL;

    node->parent = parent;
    for (int idx = 0; idx < ALLNUM; idx++)
        node->children[idx] = NULL;
    node->count = 0;
    node->lastVisited = -1;

    node->value.seq = NULL;
    node->hasList = hasList;

    node->bound = NULL;
    node->pointedBy = pointedBy;

    return node;
}

/**
 * @brief Zwalnia węzeł @p node. Jeśli z wierzchołkiem skojarzony jest
 * pewien element listy w innym drzewie, to zostaje on usunięty.
 * @param node - wskaźnik na węzeł drzewa.
 */
static void freeTrieNode(TrieNode *node) {
    if (node->hasList) {
        listDelete(node->value.list);
        node->value.list = NULL;
    }
    else {
        listNodeRemoveAndCut(node->bound);
        free(node->value.seq);
    }

    free(node);
}

void trieDelete(TrieNode *node) {
    if (!node) return;

    int idx;
    TrieNode *toFree;
    TrieNode *curr = node;

    while (curr)
        if (curr->count == 0) {
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
                curr->count--;
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
        if (!(node->value.list = listInit(value, length, node))) return NULL;
        else return listNodeHead(node->value.list);
    }

    return listAdd(node->value.list, value, length);
}

bool trieNodeBind(TrieNode *trieNode, ListNode *listNode) {
    if (!listNode || !trieNode || trieNode->hasList) return false;
    listNodeRemove(trieNode->bound);
    trieNode->bound = listNode;
    return true;
}

bool trieNodeSetSeq(TrieNode *node, const char *seq, size_t length) {
    if (!node) return false;

    node->value.seq = realloc(node->value.seq, (length + 1) * sizeof(char));
    if (!node->value.seq) return false;

    strcpy(node->value.seq, seq);
    return true;
}

List *trieGetList(TrieNode *node) {
    if (!node || !node->hasList) return NULL;
    return node->value.list;
}

TrieNode *trieGetParent(TrieNode *node) {
    if (!node) return NULL;
    return node->parent;
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
        idx = getValue(str[i]);
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
    if (!(*rootPtr)) *rootPtr = trieNodeNew(NULL, hasList, rootPtr);
    if (!(*rootPtr)) return NULL;

    int idx;
    TrieNode *v = *rootPtr;

    for (size_t i = 0; str[i] != '\0'; i++) {
        idx = getValue(str[i]);
        if (!v->children[idx]) {
            v->children[idx] = trieNodeNew(v, hasList, &v->children[idx]);
            if (!v->children[idx]) return NULL;
            v->count++;
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
        int idx;
        bool mayExist = true;
        for (size_t i = 0; mayExist && str[i] != '\0'; i++) {
            idx = getValue(str[i]);
            if (!v || !v->children[idx]) mayExist = false;
            else
                v = v->children[idx];
        }
        if (mayExist) {
            *v->pointedBy = NULL;
            v->parent->count--;
            trieCutLeaves(v->parent);
            trieDelete(v);
        }
    }
}

/**
 * @brief Sprawdza, czy węzeł @p node jest pusty.
 * @param node - wskaźnik na węzeł drzewa.
 * @return Wartość @p true pod warunkiem, że węzeł nie ma dzieci i nie ma
 * znaczącej zawartości. Wartość @p false w przeciwnym wypadku.
 */
static bool trieNodeIsEmpty(TrieNode *node) {
    if (node->count) return false;
    if (node->hasList)
        return !node->value.list || isEmpty(node->value.list);
    else
        return !node->value.seq;
}

void trieCutLeaves(TrieNode *node) {
    if (!node) return;

    TrieNode *curr = node, *next;
    while (curr && trieNodeIsEmpty(curr)) {
        if (curr->hasList)
            listDelete(curr->value.list);

        *curr->pointedBy = NULL;
        if (curr->parent) {
            curr->parent->count--;
            curr->parent->lastVisited = -1;
        }

        next = curr->parent;
        free(curr);
        curr = next;
    }
}
