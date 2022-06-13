/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>
#include <string.h>
#include "phone_forward.h"
#include "trie.h"
#include "alphabet.h"
#include "dynamic_table.h"

/** @brief Struktura przechowująca przekierowania telefonów.
 * Struktura przechowująca przekierowania telefonów trzyma je w postaci
 * węzłów w drzewie trie @p fwds. Prefiksy zaczynają się w korzeniu drzewa i kończą w
 * odpowiednich węzłach. Jeśli dla danego prefiksu ustalono przekierowanie,
 * to wartością w węźle jest ciąg znaków go reprezentujący.
 *
 * Węzeł drzewa @p revs stanowi ustalone przekierowanie, zaś jego wartościami
 * są skojarzone z tym przekierowaniem prefiksy, przechowywane w tablicy. Np. efektem
 * wywołania operacji
 * @code
 * phfwdAdd(pf, "2", "4");
 * phfwdAdd(pf, "23", "4");
 * @endcode
 * będzie umieszczenie w @p revs w wierzchołku "4" prefiksów "2" oraz "23".
 * @see trie.h
 */
struct PhoneForward {
    TrieNode *fwds; /**< Wskaźnik na korzeń struktury przechowującej jako węzły
                         prefiksy, dla których ustalono przekierowanie. */
    TrieNode *revs; /**< Wskaźnik na korzeń struktury przechowującej jako węzły
                         przekierowania. */
};

/**
 * Struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneNumbers {
    Table *nums; /**< Dynamiczna tablica w której przechowywane
                      są numery telefonów. */
};

PhoneForward *phfwdNew(void) {
    PhoneForward *pf = malloc(sizeof(PhoneForward));
    if (!pf) return NULL;

    pf->fwds = trieNodeNew(NULL, false, &pf->fwds);
    pf->revs = NULL;

    if (!pf->fwds) {
        free(pf);
        return NULL;
    }

    return pf;
}

void phfwdDelete(PhoneForward *pf) {
    if (!pf) return;
    trieDelete(pf->fwds);
    trieDelete(pf->revs);
    free(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if (!pf) return false;
    size_t len1, len2;
    if (!(len1 = isCorrect(num1)) || !(len2 = isCorrect(num2))) return false;
    if (strcmp(num1, num2) == 0) return false;

    if (!pf->revs) pf->revs = trieNodeNew(NULL, true, &pf->revs);
    if (!pf->revs) return false;

    TrieNode *fwd = trieInsertStr(&(pf->fwds), num1, false);
    TrieNode *rev = trieInsertStr(&(pf->revs), num2, true);

    if (!fwd || !rev) return false;

    return trieNodeSetSeq(fwd, num2, len2) &&
           trieNodeBind(fwd, trieAddToList(rev, num1, len1));
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if (pf && isCorrect(num))
        trieRemoveStr(&(pf->fwds), num);
}

/**
 * @brief Alokuje nową strukturę @p PhoneNumbers.
 * @return Wskaźnik na strukturę @p PhoneNumbers lub NULL, gdy nie udało się
 * alokować pamięci.
 */
static PhoneNumbers *phnumNew() {
    PhoneNumbers *pnum = malloc(sizeof(PhoneNumbers));
    if (!pnum) return NULL;

    pnum->nums = tableNew();
    if (!pnum->nums) {
        free(pnum);
        return NULL;
    }

    return pnum;
}

/**
 * @brief Dodaje ciąg znaków @p num do @p pnum.
 * @return Wartość @p true, jeśli operacja się powiodła, wartość @p false w
 * przeciwnym wypadku.
 */
static bool phnumAdd(PhoneNumbers *pnum, char *num) {
    if (!pnum) return false;
    return tableAddPtr(pnum->nums, num);
}

/**
 * @brief Zmienia prefiks podanego numeru na nowy.
 * Alokuje tablicę znaków o długości nowego numeru. Wstawia do niej nowy
 * prefiks a po nich cyfry, które pozostają bez zmian. Zakłada poprawność
 * wszystkich danych wejściowych.
 * @param[in] num - wskaźnik na numer.
 * @param[in] fwdPrefix - wskaźnik na nowy prefiks.
 * @param[in] numLength - długość numeru.
 * @param[in] toReplace - liczba początkowych znaków numeru, które zastępuje
 *                        nowy prefiks.
 * @return Wskaźnik na napis reprezentujący numer telefonu z nowym prefiksem
 * lub NULL, gdy nie udało się alokować pamięci.
 */
static char *
replacePrefix(char const *num, char const *fwdPrefix, size_t numLength,
              size_t toReplace) {
    size_t fwdPrefixLength = strlen(fwdPrefix);
    size_t newNumLength = numLength + fwdPrefixLength - toReplace;

    char *new = calloc((newNumLength + 1), sizeof(char));
    if (!new) return NULL;

    size_t i = 0, j = 0;

    while (i + j < newNumLength)
        if (i < fwdPrefixLength) {
            new[i] = fwdPrefix[i];
            i++;
        } else {
            new[fwdPrefixLength + j] = num[j + toReplace];
            j++;
        }

    return new;
}

static PhoneNumbers *phnumWithOne(PhoneNumbers *pnum, const char *num, size_t
length) {
    if (!pnum) pnum = phnumNew();
    char *toAdd = malloc((length + 1) * sizeof(char));
    if (!toAdd || !strcpy(toAdd, num) || !phnumAdd(pnum, toAdd)) {
        free(toAdd);
        phnumDelete(pnum);
        return NULL;
    }
    return pnum;
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    if (!pf) return NULL;

    PhoneNumbers *pnum = phnumNew();
    if (!pnum) return NULL;

    size_t length = isCorrect(num);
    if (!length) return pnum;

    size_t toReplace;
    TrieNode *found = trieFindSeq(pf->fwds, num, &toReplace);
    if (!found) return phnumWithOne(pnum, num, length);

    char *replaced = replacePrefix(num, trieNodeGetSeq(found), length,
                                   toReplace);
    if (!replaced) {
        phnumDelete(pnum);
        return NULL;
    }

    phnumAdd(pnum, replaced);

    return pnum;
}

/** @brief Zbiera wskaźniki do nowych prefiksów ze wszystkich list na ścieżce z
 * wierzchołka @p from do korzenia drzewa w którym się znajduje.
 * Alokuje strukturę @p Table, która musi być zwolniona za pomocą funkcji @ref
 * tableFreeAll. Umieszcza w @p Table numery powstałe z
 * @p num z zastąpionymi odpowiednimi prefiksami.
 * @param from - wskaźnik na początek ścieżki.
 * @param num - wskaźnik na ciąg znaków, którego prefiksy będą zastępowane nowymi.
 * @param length - długość @p num.
 * @param depth - głębokość wierzchołka, czyli długość w.w. ścieżki.
 * @return Wskaźnik na strukturę @p Table, lub NULL, gdy nie udało się
 * alokować pamięci.
 */
static Table *
findAllRevs(TrieNode *from, char const *num, size_t length, size_t depth) {
    Table *revs = tableNew();
    if (!revs) return NULL;

    TrieNode *curr = from;
    char **arr = NULL;
    size_t size;
    char *replaced;

    while (curr) {
        free(arr);
        arr = listToArray(trieGetList(curr), &size);
        if (arr) {
            qsort(arr, size, sizeof(char *), strCompare);
            for (size_t i = 0; i < size; i++) {
                replaced = replacePrefix(num, arr[i], length, depth);
                if (!tableAddPtr(revs, replaced)) {
                    free(arr);
                    free(replaced);
                    tableFreeAll(revs);
                    return NULL;
                }
            }
        }
        curr = trieGetParent(curr);
        depth--;
    }

    if (tableIsEmpty(revs)) {
        /* Tablica @p revs jest pusta, co jest sprzeczne z założeniem. Musiał
        więc nastąpić problem z alokacją pamięci w listToArray() */
        tableFreeAll(revs);
        return NULL;
    }

    return revs;
}

/**
 * @brief Dodaje do struktury @p pnum wszystkie rozróżnialne elementy z
 * tablicy @p duplicated. Dodawane elementy są posortowane leksykograficzne.
 * Dopuszcza, że w @p duplicated istnieją duplikaty. Zwalnia tablicę @p
 * duplicated i tylko wskazywane przez nią duplikaty, w szczególności nie
 * zwalnia dodanych do @p pnum elementów,
 * @param pnum - wskaźnik na strukturę, do której dodawane są elementy.
 * @param duplicated - wskaźnik na tablicę, z której dodawane będą elementy.
 */
static bool phnumConsumeDistinct(PhoneNumbers *pnum, Table *duplicated) {
    tableSort(duplicated, strCompare);

    char *prev = NULL;
    char *elem;

    for (size_t i = 0; i < tableGetAmount(duplicated); i++) {
        elem = tableGet(duplicated, i);
        if (!prev || strCompare(&elem, &prev) != 0) {
            prev = elem;
            if (!phnumAdd(pnum, tableGet(duplicated, i))) {
                return false;
            }
        } else free(elem);
    }
    tableFree(duplicated);
    return true;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    size_t length, toReplace = 0;
    if (!pf) return NULL;
    if (!(length = isCorrect(num))) return phnumNew();

    TrieNode *longest = trieFindSeq(pf->revs, num, &toReplace);
    if (!longest) return phnumWithOne(NULL, num, length);

    Table *duplicated = findAllRevs(longest, num, length, toReplace);

    PhoneNumbers *pnum = phnumNew();
    if (!pnum || !duplicated || !tableAdd(duplicated, num) ||
        !phnumConsumeDistinct(pnum, duplicated)) {
        tableFreeAll(duplicated);
        return phnumWithOne(pnum, num, length);
    }

    return pnum;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (!pnum) return;
    tableFreeAll(pnum->nums);
    free(pnum);
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (!pnum) return NULL;
    return tableGet(pnum->nums, idx);
}
