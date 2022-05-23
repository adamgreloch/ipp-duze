/** @file
 * Interfejs klasy obsługującej strukturę danych trie dla ciągów znaków
 * ze zbioru \f$\Omega=\{0,1,...,9\}\cup\{*, \#\}\f$ (zwanych odtąd
 * poprawnymi ciągami znaków).
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>
#include <stddef.h>

#define ALLNUM 12 /**< Rozmiar alfabetu w drzewie,
                       czyli moc zbioru \f$\Omega\f$. */

/**
 * Struktura przechowująca węzeł drzewa trie.
 */
struct TrieNode {
    struct TrieNode *parent; /**< Wskaźnik na rodzica węzła. */
    struct TrieNode *children[ALLNUM]; /**< Tablica wskaźników na dzieci węzła. */
    bool isTerminal; /**< Wartość logiczna przyjmująca TRUE, jeśli węzeł
                          jest liściem, FALSE w przeciwnym wypadku. */
    char *seq; /**< Wartość węzła, która, jeśli niepusta (ma wartość inną niż
                      NULL), to jest poprawnym ciągiem znaków. */
    int lastVisited; /**< Ostatnio odwiedzony przez trieDelete() numer
                          dziecka. Resetowany do -1 przy zmianie
                          struktury poddrzew. */
    char **multiple; /**< Wartość węzła, która, jeśli niepusta, to jest tablicą
                          poprawnych ciągów znaków */
    char **seqPtr; /**< Wskaźnik na poprawny ciąg znaków, znajdujący się w
                       @p multiple pewnego węzła. */
};
typedef struct TrieNode TrieNode; /**< @struct TrieNode */

/** @brief Tworzy nowy węzeł.
 * Tworzy nowy węzeł @p TrieNode o pustej wartości. Ustawia jego wskaźnik
 * rodzica na @p parent. Powstały węzeł musi być zwolniony za pomocą funkcji
 * trieDelete().
 * @param[in] parent - wskaźnik na węzeł rodzica.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * alokować pamięci.
 */
TrieNode *trieNodeNew(TrieNode *parent);

/** @brief Usuwa drzewo zakorzenione w @p node.
 * Usuwa drzewo trie zakorzenione w węźle @p node. Zwalnia jego pamięć.
 * @param[in,out] node - wskaźnik na korzeń drzewa do usunięcia.
 */
void trieDelete(TrieNode *node);

/** @brief Ustawia wartość w węźle @p node na @p value.
 * Ustawia wartość w węźle @p node drzewa trie na @p value. Zakłada
 * poprawność @p value.
 * @param[in,out] node - wskaźnik na węzeł w którym modyfikowana jest wartość.
 * @param[in] value - ustawiany ciąg znaków.
 * @param[in] length - długość @p value (nie licząc znaku terminującego).
 * @return Wartość @p true, jeśli wartość została ustawiona. Wartość @p
 * false, jeśli @p node ma wartość NULL, bądź nie udało się alokować pamięci.
 */
bool trieNodeSet(TrieNode *node, const char *value, size_t length);

/** @brief Zwraca wartość węzła @p node.
 * @param[in] node - wskaźnik na oglądany węzeł.
 * @return Wartość w @p node, bądź NULL, jeśli @p node ma wartość NULL.
 */
const char *trieNodeGet(TrieNode *node);

/** @brief Znajduje najdłuższy prefiks @p str o niepustej wartości w drzewie.
 * Znajduje najdłuższy prefix ciągu @p str zawierający niepustą
 * wartość w drzewie zakorzenionym w @p root. Zakłada poprawność @p str.
 * @param[in] root - wskaźnik na korzeń przeszukiwanego drzewa.
 * @param[in] str - ciąg znaków, dla którego szukany jest najdłuższy prefiks.
 * @param[in,out] length - wskaźnik na zmienną, w której zostanie zapisana
 *                         długość znalezionego prefiksu.
 * @return Wskaźnik na węzeł kończący najdłuższy prefiks lub NULL, gdy @p root
 * ma wartość NULL, lub gdy szukany prefiks nie istnieje.
 */
TrieNode *trieFind(TrieNode *root, const char *str, size_t *length);

/** @brief Umieszcza ciąg @p str w drzewie.
 * Umieszcza ciąg @p str w drzewie zakorzenionym w @p *rootPtr. Zakłada
 * poprawność @p str.
 * @param[in,out] rootPtr - podwójny wskaźnik na drzewo.
 * @param[in] str - umieszczany ciąg znaków.
 * @return Wskaźnik na węzeł kończący ciąg lub NULL, gdy nie udało się
 * alokować pamięci.
 */
TrieNode *trieInsertStr(TrieNode **rootPtr, const char *str);

/** @brief Usuwa wszystkie ciągi z drzewa, których prefiksem jest @p str.
 * Usuwa wszystkie ciągi z drzewa zakorzenionego w @p *rootPtr, których
 * prefiksem jest ciąg @p str. Zakłada poprawność @p str.
 * @param[in,out] rootPtr - podwójny wskaźnik na drzewo.
 * @param[in] str - ciąg znaków.
 */
void trieRemoveStr(TrieNode **rootPtr, const char *str);

#endif /* __TRIE_H__ */
