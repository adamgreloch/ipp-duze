/** @file
 * Interfejs klasy obsługującej listę wskaźnikową przechowującą
 * poprawne ciągi znaków (zdefiniowane w @ref alphabet.h). Lista ta ma być
 * przechowywana w wierzchołkach drzew @p TrieNode.
 * Klasa implementuje struktury @p ListNode, @p List zadeklarowane w
 * interfejsie @ref structs.h.
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

/** @brief Tworzy nową listę.
 * Tworzy nową jednoelementową listę @p List o wartości @p str.
 * Ustawia jej wskaźnik rodzica na @p owner. Powstała lista musi być zwolniona
 * za pomocą funkcji listDelete().
 * @param[in] str - wskaźnik na poprawny ciąg znaków.
 * @param[in] length - długość @p str.
 * @param[in] owner - wskaźnik na węzeł rodzica.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * alokować pamięci.
 */
List *listInit(char const *str, size_t length, TrieNode *owner);

/**
 * @brief Sprawdza, czy lista @p l jest pusta.
 * Zakłada, że @p l nie jest NULL-em.
 * @param[in] l - wskaźnik na listę.
 * @return Wartość @p true, jeśli jest pusta, wartość @p false w przeciwnym
 * wypadku.
 */
bool isEmpty(List *l);

/**
 * @brief Dodaje węzeł listy o wartości @p str do listy.
 * @param[in,out] l - wskaźnik na listę.
 * @param[in] str - wskaźnik na poprawny ciąg znaków.
 * @param[in] length - długość @p str.
 * @return Wskaźnik na dodany węzeł listy, lub NULL, gdy nie udało się
 * alokować pamięci.
 */
ListNode *listAdd(List *l, char const *str, size_t length);

/**
 * @brief Zwraca head podanej listy.
 * Zakłada, że @p l nie jest NULL-em.
 * @param[in] l - wskaźnik na listę.
 * @return Wskaźnik na head listy.
 */
ListNode *listNodeHead(List *l);

/**
 * @brief Usuwa element listy @p node z listy do której należy.
 * @param[in,out] node - wskaźnik na usuwany element.
 */
void listNodeRemove(ListNode *node);

/**
* @brief Usuwa element listy @p node z listy do której należy. Jeśli ta
 * lista stanie się pusta, to usuwa listę i przechowujący ją wierzchołek drzewa.
 * Usunięcie listy sprawia, że związany z nią wierzchołek drzewa trie staje
 * się niepotrzebny, wobec czego jest usuwany. Jeśli nad tym wierzchołkiem
 * są puste wierzchołki, to też zostają usuwane.
* @param[in,out] node - wskaźnik na usuwany element.
*/
void listNodeRemoveAndCut(ListNode *node);

/**
* @brief Usuwa listę @p l. Zwalnia jej pamięć.
* @param[in,out] l - wskaźnik na usuwaną listę.
*/
void listDelete(List *l);

/**
* @brief Tworzy tablicę poprawnych ciągów znaków reprezentującą zawartość
* listy @p l. Aktualizuje wartość pod wskaźnikiem @p arraySize do liczby
* elementów w wynikowej tablicy.
* @param[in] l - wskaźnik na listę.
* @param[out] arraySize - wskaźnik na rozmiar tworzonej tablicy.
* @return Wskaźnik na powstałą tablicę, lub NULL, gdy nie udało się alokować
 * pamięci, bądź @p l jest NULL-em.
*/
char **listToArray(List *l, size_t *arraySize);

#endif /* __LINKEDLIST_H__ */
