/** @file
 * Interfejs klasy obsługującej strukturę danych trie dla ciągów znaków
 * (zdefiniowanych w @ref alphabet.h). Klasa umożliwia utworzenie dwóch
 * wzajemnie zależnych drzew. Klasa implementuje strukturę @p TrieNode
 * zadeklarowaną w interfejsie @ref structs.h.
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>
#include <stddef.h>
#include "structs.h"
#include "linked_list.h"

/** @brief Tworzy nowy węzeł.
 * Tworzy nowy węzeł @p TrieNode o pustej wartości. Powstały węzeł musi być
 * zwolniony za pomocą funkcji trieDelete().
 * @param[in] parent - wskaźnik na węzeł rodzica.
 * @param[in] hasList - wartość wskazująca typ zawartości drzewa.
 * @param[in] pointedBy - wskaźnik na wskaźnik na ten węzeł w tablicy dzieci
 * rodzica.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * alokować pamięci.
 */
TrieNode *trieNodeNew(TrieNode *parent, bool hasList, TrieNode **pointedBy);

/** @brief Usuwa drzewo zakorzenione w @p node.
 * Usuwa drzewo trie zakorzenione w węźle @p node. Zwalnia jego pamięć.
 * @param[in,out] node - wskaźnik na korzeń drzewa do usunięcia.
 */
void trieDelete(TrieNode *node);

/** @brief Ustawia wartość w węźle @p node na ciąg znaków @p seq.
 * Ustawia wartość w węźle @p node drzewa trie na @p value. Zakłada
 * poprawność @p value.
 * @param[in,out] node - wskaźnik na węzeł w którym modyfikowana jest wartość.
 * @param[in] seq - ustawiany ciąg znaków.
 * @param[in] length - długość @p value (nie licząc znaku terminującego).
 * @return Wartość @p true, jeśli wartość została ustawiona. Wartość @p
 * false, jeśli węzeł @p node ma wartość NULL, węzeł jest niewłaściwego typu, bądź nie
 * udało się alokować pamięci.
 */
bool trieNodeSetSeq(TrieNode *node, const char *seq, size_t length);

/**
 * @brief Zwraca wskaźnik na ojca węzła @p node.
 * @param node - wskaźnik na węzeł drzewa.
 * @return Wskaźnik na ojca, lub NULL, jeśli @p node ma wartość NULL.
 */
TrieNode *trieGetParent(TrieNode *node);

/** @brief Zwraca wartość węzła @p node jako poprawny ciąg znaków.
 * @param[in] node - wskaźnik na oglądany węzeł.
 * @return Wartość w @p node, bądź NULL, jeśli węzeł @p node ma wartość NULL
 * lub węzeł jest niewłaściwego typu.
 */
const char *trieNodeGetSeq(TrieNode *node);

/** @brief Zwraca wskaźnik na listę w węźle @p node.
 * @param[in] node - wskaźnik na oglądany węzeł.
 * @return Wskaźnik na listę, bądź NULL, jeśli węzeł @p node ma
 * wartość NULL, lub węzeł jest niewłaściwego typu.
 */
List *trieGetList(TrieNode *node);

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
TrieNode *trieFindSeq(TrieNode *root, const char *str, size_t *length);

/** @brief Umieszcza ciąg @p str w drzewie.
 * Umieszcza ciąg @p str w drzewie zakorzenionym w @p *rootPtr. Zakłada
 * poprawność @p str.
 * @param[in,out] rootPtr - podwójny wskaźnik na drzewo.
 * @param[in] str - umieszczany ciąg znaków.
 * @param[in] hasList - wartość wskazująca typ drzewa.
 * @return Wskaźnik na węzeł kończący ciąg lub NULL, gdy nie udało się
 * alokować pamięci.
 */
TrieNode *trieInsertStr(TrieNode **rootPtr, const char *str, bool hasList);

/** @brief Usuwa wszystkie ciągi z drzewa, których prefiksem jest @p str.
 * Usuwa wszystkie ciągi z drzewa zakorzenionego w @p *rootPtr, których
 * prefiksem jest ciąg @p str. Zakłada poprawność @p str. Usuwa również
 * wszelkie zbędne węzły od @p *rootPtr do korzenia, jeśli takie napotka.
 * @param[in,out] rootPtr - podwójny wskaźnik na drzewo.
 * @param[in] str - ciąg znaków.
 */
void trieRemoveStr(TrieNode **rootPtr, const char *str);

/** @brief Dodaje ciąg znaków @p value do listy znajdującej się w węźle @p node.
 * Zakłada poprawność @p value. Jeśli @p node pozwala na przechowywanie list,
 * lecz jej nie zawiera, to funkcja alokuje w nim nową listę @p List.
 * @param[in,out] node - wskaźnik na węzeł w którym modyfikowana jest wartość.
 * @param[in] value - ustawiany ciąg znaków.
 * @param[in] length - długość @p value (nie licząc znaku terminującego).
 * @return Wartość @p true, jeśli wartość została ustawiona. Wartość @p
 * false, jeśli węzeł @p node ma wartość NULL, węzeł jest niewłaściwego
 * typu, bądź nie udało się alokować pamięci.
 */
ListNode *trieAddToList(TrieNode *node, const char *value, size_t length);

/**
 * @brief Dodaje do @p trieNode wskaźnik na węzeł listy @p listNode.
 * Zakłada, że @p trieNode jest drzewem typu @p value.seq.
 * @remark Ponieważ dany węzeł listy z założenia należy do pewnego
 * drzewa typu @p value.list, to funkcja ta w rzeczywistości tworzy powiązanie
 * między dwoma węzłami po jednym z drzew różnych typów.
 * @param trieNode - wskaźnik na węzeł drzewa do którego dodawane jest
 * skojarzenie
 * @param listNode - wskaźnik na kojarzony węzeł listy.
 * @return Wartość @p true, jeśli udało się dodać skojarzenie. Wartość @p
 * false, jeśli @p trieNode jest niewłaściwego typu, lub podano pusty
 * wskaźnik.
 */
bool trieNodeBind(TrieNode *trieNode, ListNode *listNode);

/**
 * @brief Usuwa puste liście na ścieżce z @p node do korzenia drzewa do
 * którego należy @p node. Zakłada, że @p node jest dany do usunięcia.
 * @param node - wskaźnik na węzeł drzewa od którego usuwane są liście.
 */
void trieCutLeaves(TrieNode *node);

#endif /* __TRIE_H__ */
