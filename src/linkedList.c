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
    ListNode *prev;
    ListNode *next;
};

struct List {
    ListNode *head;
};

List * listInit(char const *str, size_t length) {
    List *l = malloc(sizeof(List));
    if (!l) return NULL;

    l->head = malloc(sizeof(ListNode));
    if (!l->head) {
        free(l);
        return NULL;
    }

    l->head->str = malloc((length + 1) * sizeof(char));
    if (!l->head->str) return NULL;
    strcpy(l->head->str, str);

    l->head->prev = l->head;
    l->head->next = l->head;

    return l;
}

ListNode *listAdd(List *l, char const *str, size_t length) {
    ListNode *n = malloc(sizeof(ListNode));
    if (!n) return NULL;

    n->str = malloc((length + 1) * sizeof(char));
    if (!n->str) return NULL;

    strcpy(n->str, str);

    n->prev = NULL;
    n->next = l->head;
    l->head->prev = n;
    l->head = n;

    return n;
}

ListNode *listPollNode(List *l) {
    if (!l->head->prev) return l->head->prev;
    return NULL;
}

char *listPoll(List *l) {
    if (!l->head->prev) return l->head->prev->str;
    return NULL;
}

void removeListNode(ListNode *node) {
    if (!node) return;

    ListNode *prev = node->prev;
    ListNode *next = node->next;

    free(node->str);
    free(node);

    if (prev) prev->next = next;
    if (next) next->prev = prev;
}

void listDelete(List *l) {
    if (!l) return;
    ListNode *curr = l->head;
    ListNode *next;
    while (curr) {
        next = curr->next;
        if (!next) next = curr->prev;
        free(curr->str);
        free(curr);
        curr = next;
    }
}