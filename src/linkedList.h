/** @file
 * Interfejs klasy obsługującej listę wskaźnikową przechowującą
 * poprawne ciągi znaków (zdefiniowane w trie.h).
 *
 * @see trie.h
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdlib.h>
#include <stdbool.h>

struct ListNode;

typedef struct ListNode ListNode;

struct List;

typedef struct List List;

List * listInit(char const *str, size_t length);

bool isEmpty(ListNode *l);

ListNode *listAdd(List *l, char const *str, size_t length);

char *listPoll(List *l);

ListNode *listPollNode(List *l);

void removeListNode(ListNode *node);

void listDelete(List *l);

#endif /* __LINKEDLIST_H__ */
