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

ListNode *listAdd(List *l, char const *str, size_t length) {
    char *res = malloc((length + 1) * sizeof(char));
    if (!res) return NULL;

    strcpy(res, str);

    if (!l->elem->str) {
        l->elem->str = res;
        return l->elem;
    }

    ListNode *n = malloc(sizeof(ListNode));
    if (!n) {
        free(res);
        return NULL;
    }

    n->str = res;

    l->elem->prev->next = n;
    n->prev = l->elem->prev;
    n->next = NULL;
    l->elem->prev = n;

    return n;
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
    if (next) next->prev = prev;
}

void listDelete(List *l) {
    if (!l) return;
    ListNode *curr = l->elem;
    while (curr->next) {
        // TODO coś się nie NULL-uje jak powinno
        free(curr->str);
        curr = curr->next;
        free(curr->prev);
    }
    free(l);
}
