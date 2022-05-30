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
#include "structs.h"

List * listInit(char const *str, size_t length, TrieNode *owner);

bool isEmpty(List *l);

ListNode *listAdd(List *l, char const *str, size_t length);

char *listPeek(List *l);

ListNode *listNodePeek(List *l);

void listNodeRemove(ListNode *node);

void listNodeRemoveAndCut(ListNode *node);

void listDelete(List *l);

#endif /* __LINKEDLIST_H__ */
