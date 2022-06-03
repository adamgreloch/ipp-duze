/** @file
 * Implementacja klasy obsługującej listę wskaźnikową przechowującą
 * poprawne ciągi znaków (zdefiniowane w @ref alphabet.h). Lista ta ma być
 * przechowywana w wierzchołkach drzew @p TrieNode.
 * Klasa implementuje struktury @p ListNode, @p List zadeklarowane w
 * interfejsie @ref structs.h.
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <string.h>
#include "linked_list.h"
#include "trie.h"

/**
 * Struktura przechowująca element listy typu @p List.
 */
struct ListNode {
    char *str; /**< Wskaźnik na poprawny ciąg znaków. */
    ListNode *prev; /**< Wskaźnik na poprzedni węzeł listy. */
    ListNode *next; /**< Wskaźnik na następny węzeł listy. */
    List *parent; /**< Wskaźnik na listę zawierającą ten węzeł. */
};

/**
 * Struktura podwójnie łączoną listę elementów typu @p ListNode.
 */
struct List {
    ListNode *head; /**< Wskaźnik na head listy. */
    TrieNode *owner; /**< Wskaźnik na węzeł drzewa trie zawierającego
                          wskaźnik na tę listę. */
};

List *listInit(char const *str, size_t length, TrieNode *owner) {
    if (!owner) return NULL;

    List *l = malloc(sizeof(List));
    if (!l) return NULL;

    l->head = malloc(sizeof(ListNode));
    if (!l->head) {
        free(l);
        return NULL;
    }

    l->head->str = malloc((length + 1) * sizeof(char));
    if (!l->head->str) {
        free(l->head);
        free(l);
        return NULL;
    }
    strcpy(l->head->str, str);

    l->head->prev = NULL;
    l->head->next = NULL;
    l->head->parent = l;
    l->owner = owner;

    return l;
}

ListNode *listAdd(List *l, char const *str, size_t length) {
    if (!l) return NULL;

    ListNode *n = malloc(sizeof(ListNode));
    if (!n) return NULL;

    n->str = malloc((length + 1) * sizeof(char));
    if (!n->str) {
        free(n);
        return NULL;
    }

    strcpy(n->str, str);

    n->parent = l;
    n->prev = NULL;
    n->next = l->head;

    if (l->head) l->head->prev = n;
    l->head = n;

    return n;
}

ListNode *listNodeHead(List *l) {
    return l->head;
}

void listNodeRemove(ListNode *node) {
    if (!node || !node->parent) return;

    List *parent = node->parent;

    if (node == parent->head)
        parent->head = node->next;

    if (node->next)
        node->next->prev = node->prev;

    if (node->prev)
        node->prev->next = node->next;

    free(node->str);
    free(node);
}

char **listToArray(List *l, size_t *arraySize) {
    if (!l) return NULL;

    size_t size = 0;
    ListNode *curr = l->head;
    if (!curr) return NULL;

    while (curr) {
        size++;
        curr = curr->next;
    }
    curr = l->head;

    *arraySize = size;

    char **res = malloc(size * sizeof(char *));
    if (!res) return NULL;

    size_t i = 0;
    while (curr) {
        res[i] = curr->str;
        curr = curr->next;
        i++;
    }

    return res;
}

void listNodeRemoveAndCut(ListNode *node) {
    if (!node) return;
    List *parent = node->parent;
    listNodeRemove(node);
    if (isEmpty(parent))
        trieCutLeaves(parent->owner);
}

void listDelete(List *l) {
    if (!l) return;
    ListNode *curr = l->head;
    ListNode *next;
    while (curr) {
        next = curr->next;
        listNodeRemove(curr);
        curr = next;
    }
    free(l);
}

bool isEmpty(List *l) {
    return !l->head;
}
