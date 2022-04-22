/** @file
 * Interfejs klasy obsługującej strukturę danych trie
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>
#include <stddef.h>

#define ALLNUM 10 // Rozmiar "alfabetu" {0,1,...,9}

struct TrieNode;
typedef struct TrieNode TrieNode;

struct TrieNode {
    TrieNode *parent;
    TrieNode **children;
    bool isTerminal;
    char *value;
};

// TODO dokumentacja

TrieNode *trieNodeNew(TrieNode *parent);

void trieDelete(TrieNode *node);

bool trieNodeSet(TrieNode *node, const char *value);

const char *trieNodeGet(TrieNode *node);

TrieNode *trieFind(TrieNode *v, const char *prefix, size_t *length);

TrieNode *trieInsert(TrieNode **rootPtr, const char *str);

void trieRemoveStr(TrieNode **rootPtr, const char *str);

#endif /* __TRIE_H__ */
