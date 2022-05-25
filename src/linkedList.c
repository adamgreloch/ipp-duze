/** @file
 * Implementacja klasy obsługującej dwukierunkową listę wskaźnikową
 * przechowującą poprawne ciągi znaków (zdefiniowane w trie.h).
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include "linkedList.h"

struct ListNode {
    char *str; /**< Poprawny ciąg znaków. */
    struct ListNode *prev;
    struct ListNode *next;
};

List *listInit() {
    List *l = malloc(sizeof(List));
    if (!l) return NULL;
    l->elem = malloc(sizeof(ListNode));

    if (!l->elem) {
        free(l);
        return NULL;
    }

    l->elem->str = NULL;
    l->elem->prev = l->elem;
    l->elem->next = NULL;

    return l;
}

bool listAdd(List *l, char *str) {
    if (!l->elem->str) {
        l->elem->str = str;
        return true;
    }

    ListNode *n = malloc(sizeof(ListNode));
    if (!n) return false;
    n->str = str;

    ListNode *e = l->elem;

    e->prev->next = n;
    n->prev = e->prev;
    n->next = NULL;
    e->prev = n;

    return true;
}

char *listPoll(List *l) {
    if (!l->elem->prev) return l->elem->prev->str;
    return NULL;
}

void removeListNode(ListNode *node) {
    if (!node) return;
    ListNode *prev = node->prev;
    ListNode *next = node->next;

    free(node->str);
    free(node);

    prev->next = next;
    next->prev = prev;
}

void listDelete(List *l) {
    if (!l) return;
    ListNode *curr = l->elem;
    while (curr->next) {
        free(curr->str);
        curr = curr->next;
        free(curr->prev);
    }
    free(l);
}
