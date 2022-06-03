/** @file
 * Interfejs klasy obsługującej dynamiczną tablicę poprawnych ciągów
 * znaków (zdefiniowanych w @ref alphabet.h)
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef __DYNAMIC_TABLE_H__
#define __DYNAMIC_TABLE_H__

struct Table;

typedef struct Table Table; /**< @struct Table */

/**
 * @brief Tworzy pustą tablicę o początkowym rozmiarze @p INIT_SIZE.
 * @return Wskaźnik na tablicę, lub NULL, gdy nie udało się alokować pamięci.
 */
Table *tableNew();

/**
 * @brief Sortuje tablicę @p t za pomocą podanego komparatora @p cmp.
 * @param t - wskaźnik na tablicę do posortowania.
 * @param cmp - wskaźnik na komparator.
 */
void tableSort(Table *t, int (*cmp)(const void *, const void *));

/** @brief Dodaje na koniec tablicy @p t kopię poprawnego ciągu znaków @p str
 * Jeśli tablica jest zapełniona, podwaja jej rozmiar.
 * @param[in, out] t – wskaźnik na tablicę.
 * @param[in] str - wskaźnik na dodawany poprawny ciąg znaków.
 * @return Wartość @p true, jeśli udało się dodać ciąg. Wartość @p false,
 * jeśli nie udało się alokować pamięci.
 */
bool tableAdd(Table *t, const char *str);

/** @brief Dodaje na koniec tablicy @p t wskaźnik na poprawny ciąg znaków @p
 * str. Jeśli tablica jest zapełniona, podwaja jej rozmiar.
 * @param[in, out] t – wskaźnik na tablicę.
 * @param[in] str - wskaźnik na dodawany poprawny ciąg znaków.
 * @return Wartość @p true, jeśli udało się dodać ciąg. Wartość @p false,
 * jeśli nie udało się alokować pamięci.
 */
bool tableAddPtr(Table *t, char *str);

/** @brief Zwraca poprawny ciąg znaków zawarty w @p t pod indeksem @p idx.
 * @param[in] t – wskaźnik na tablicę.
 * @param[in] idx - indeks tablicy.
 * @return Wskaźnik na poprawny ciąg znaków, lub NULL, jeśli @p t ma wartość
 * NULL, bądź @p idx wykracza poza liczbę elementów w tablicy.
 */
char *tableGet(Table *t, size_t idx);

/** @brief Usuwa tablicę
 * Usuwa tablicę wskazywaną przez @p t. Nic nie robi, jeśli wskaźnik ten ma
 * wartość NULL.
 * @param[in,out] t – wskaźnik na usuwaną tablicę.
 */
void tableDelete(Table *t);

/**
 * @brief Zwraca liczbę elementów w tablicy @p t.
 * @param t - wskaźnik na tablicę.
 * @return Wartość nieujemna reprezentująca rozmiar tablicy. Wartość @p -1,
 * jeśli wskaźnik @p t ma wartość NULL.
 */
size_t tableGetAmount(Table *t);

#endif /* __DYNAMIC_TABLE_H__ */
