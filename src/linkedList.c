/** @file
 * Implementacja klasy obsługującej dwukierunkową listę wskaźnikową
 * przechowującą poprawne ciągi znaków (zdefiniowane w trie.h).
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <string.h>
#include "linkedList.h"
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
    size_t count;
};

List * listInit(char const *str, size_t length, TrieNode *owner) {
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

    l->head->prev = l->head;
    l->head->next = l->head;
    l->count = 1;
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

    n->prev = NULL;
    n->next = l->head;
    n->parent = l;
    l->head->prev = n;
    l->head = n;
    l->count++;

    return n;
}

ListNode *listPeekNode(List *l) {
    return l->head;
}

char *listPeek(List *l) {
    if (!l->head) return l->head->str;
    return NULL;
}

void removeListNode(ListNode *node) {
    if (!node) return;

    ListNode *prev = node->prev;
    ListNode *next = node->next;

    if (prev) prev->next = next;
    if (next) next->prev = prev;

    node->parent->count--;

    if (isEmpty(node->parent))
        trieCutLeafs(node->parent->owner);

    free(node->str);
    free(node);
}

void listDelete(List *l) {
    if (!l) return;
    if (isEmpty(l)) {
        free(l);
        return;
    }

    ListNode *curr = l->head;
    ListNode *next;
    while (!isEmpty(l)) {
        next = curr->next;
        curr->parent->count--;
        free(curr->str);
        free(curr);
        curr = next;
    }
    free(l);
}

bool isEmpty(List *l) {
    return !l || l->count == 0;
}