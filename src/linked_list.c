/** @file
 * Implementacja klasy obsługującej dwukierunkową listę wskaźnikową
 * przechowującą poprawne ciągi znaków (zdefiniowane w trie.h).
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <string.h>
#include "linked_list.h"
#include "trie.h"

struct ListNode {
    char *str; /**< Poprawny ciąg znaków. */
    ListNode *prev;
    ListNode *next;
    List *parent;
};

struct List {
    ListNode *head;
    TrieNode *owner;
};

List * listInit(char const *str, size_t length, TrieNode *owner) {
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

ListNode *listNodePeek(List *l) {
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

char** listToArray(List *l, size_t *arraySize) {
    if (!l) return NULL;

    size_t size = 0;
    ListNode *curr = l->head;
    if (!curr) return NULL;

    while (curr) {
        size++;
        curr = curr->next;
    }
    curr = l->head;

    *arraySize = size + 1;

    char** res = malloc((*arraySize) * sizeof(char*)); // TODO wytłumaczyć powód +1
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
    if (isEmpty(parent)) {
        trieCutLeaves(parent->owner);
    }
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
