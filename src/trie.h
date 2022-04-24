/** @file
 * Interfejs klasy obsługującej strukturę danych trie dla ciągów znaków
 * w systemie dziesiętnym.
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>
#include <stddef.h>

#define ALLNUM 10 /**< Rozmiar alfabetu w drzewie,
                       czyli moc zbioru \f$\{0,1,...,9\}\f$. */

/**
 * Struktura przechowująca węzeł tworzący drzewo trie.
 */
struct TrieNode {
    struct TrieNode *parent; /**< Wskaźnik na rodzica węzła. */
    struct TrieNode **children; /**< Tablica wskaźników na dzieci węzła. */
    bool isTerminal; /**< Wartość logiczna przyjmująca TRUE, jeśli węzeł
                          jest liściem, FALSE w przeciwnym wypadku. */
    char *value; /**< Wartość węzła. Jeśli nie jest pusta (ma wartość inną niż
                      NULL), to jest poprawnym ciągiem cyfr. */
};
typedef struct TrieNode TrieNode;

/** @brief Tworzy nowy węzeł.
 * Tworzy nowy węzeł @p TrieNode o pustej wartości. Ustawia jego wskaźnik
 * rodzica na @p parent. Powstały węzeł musi być zwolniony za pomocą funkcji
 * trieDelete().
 * @param[in,out] parent - wskaźnik na węzeł rodzica.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
TrieNode *trieNodeNew(TrieNode *parent);

/** @brief Usuwa drzewo zakorzenione w @p node.
 * Usuwa drzewo trie zakorzenione w węźle @p node, zwalnia całą pamięć
 * związanej z nim struktury.
 * @param[in,out] node - wskaźnik na korzeń drzewa do usunięcia.
 */
void trieDelete(TrieNode *node);

/** @brief Ustawia wartość w węźle @p node na @p value.
 * Ustawia wartość w węźle @p node drzewa trie na @p value. Zakłada
 * poprawność @p value.
 * @param[in,out] node - wskaźnik na węzeł w którym modyfikowana jest wartość.
 * @param[in] value - ustawiany ciąg znaków.
 * @return Wartość @p true, jeśli wartość została ustawiona.
 *         Wartość @p false, jeśli @p node ma wartość NULL, bądź nie udało się
 *         alokować pamięci.
 */
bool trieNodeSet(TrieNode *node, const char *value);

/** @brief Zwraca wartość w węźle @p node.
 * @param[in] node - wskaźnik na oglądany węzeł.
 * @return Wartość w @p node, bądź NULL, jeśli @p node ma wartość NULL.
 */
const char *trieNodeGet(TrieNode *node);

/** @brief Znajduje najdłuższy prefiks @p str o niepustej wartości w drzewie.
 * Znajduje najdłuższy prefix ciągu @p str zawierający niepustą
 * wartość w drzewie zakorzenionym w @p v. Zakłada poprawność @p str.
 * @param[in] v - wskaźnik na korzeń przeszukiwanego drzewa.
 * @param[in] str - ciąg znaków, dla którego szukany jest najdłuższy prefiks.
 * @param[in,out] length - wskaźnik na zmienną, w której zostanie zapisana
 * długość znalezionego prefiksu.
 * @return Wskaźnik na węzeł kończący najdłuższy prefiks lub NULL, gdy @p v ma
 * wartość NULL, lub gdy szukany prefiks nie istnieje.
 */
TrieNode *trieFind(TrieNode *v, const char *str, size_t *length);

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
