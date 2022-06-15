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
static bool phnumAdd(PhoneNumbers *pnum, const char *num) {
    if (!pnum) return false;
    return tableAdd(pnum->nums, num);
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
        }
        else {
            new[fwdPrefixLength + j] = num[j + toReplace];
            j++;
        }

    return new;
}

/**
 * Alokuje strukturę @p PhoneNumbers i umieszcza w niej tylko ciąg @p num.
 * Zakłada poprawność tego ciągu.
 * @param pnum - wskaźnik na strukturę;
 * @param num - poprawny ciąg znaków reprezentujący numer.
 * @return Wskaźnik na strukturę przechowującą podany numer.
 */
static PhoneNumbers *phnumWithOne(PhoneNumbers *pnum, const char *num) {
    if (!pnum) pnum = phnumNew();
    if (!phnumAdd(pnum, num)) {
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
    if (!found) return phnumWithOne(pnum, num);

    char *replaced = replacePrefix(num, trieNodeGetSeq(found), length,
                                   toReplace);

    if (!replaced || !phnumAdd(pnum, replaced)) {
        free(replaced);
        phnumDelete(pnum);
        return NULL;
    }

    free(replaced);
    return pnum;
}

/** @brief Zbiera wskaźniki do nowych prefiksów ze wszystkich list na ścieżce z
 * wierzchołka @p from do korzenia drzewa w którym się znajduje.
 * Umieszcza w @p revs numery powstałe z * @p num z zastąpionymi
 * odpowiednimi prefiksami.
 * @param[in] from - wskaźnik na początek ścieżki.
 * @param[in,out] revs - wskaźnik na docelową tablicę
 * @param[in] num - wskaźnik na ciąg znaków, którego prefiksy będą
 *                  zastępowane nowymi.
 * @param[in] length - długość @p num.
 * @param[in] depth - głębokość wierzchołka, czyli długość w.w. ścieżki.
 * @return Wartość @p true, jeśli operacja się powiodła. Wartość @p false, jeśli
 * nie udało sie alokować pamięci.
 */
static bool
findAllRevs(TrieNode *from, Table *revs, char const *num, size_t length,
            size_t depth) {
    TrieNode *curr = from;
    char **arr = NULL;
    size_t size;
    char *replaced;

    while (curr) {
        free(arr);
        arr = NULL;

        if (trieGetList(curr) && !isEmpty(trieGetList(curr))) {
            arr = listToArray(trieGetList(curr), &size);
            if (!arr) return false;
            qsort(arr, size, sizeof(char *), strCompare);
            for (size_t i = 0; i < size; i++) {
                replaced = replacePrefix(num, arr[i], length, depth);
                if (!tableAddPtr(revs, replaced)) {
                    free(arr);
                    free(replaced);
                    return false;
                }
            }
        }

        curr = trieGetParent(curr);
        depth--;
    }

    return true;
}

/**
 * @brief Dodaje do struktury @p pnum wszystkie rozróżnialne elementy z
 * tablicy @p duplicated. Dodawane elementy są posortowane leksykograficzne.
 * Dopuszcza, że w @p duplicated istnieją duplikaty.
 * @param[in,out] pnum - wskaźnik na strukturę, do której dodawane są elementy.
 * @param[in] duplicated - wskaźnik na tablicę, z której dodawane będą elementy.
 * @return Wartość @p true, jeśli operacja się powiodła. Wartość @p false, jeśli
 * nie udało sie alokować pamięci.
 */
static bool phnumConsumeDistinct(PhoneNumbers *pnum, Table *duplicated) {
    tableSort(duplicated, strCompare);

    char *prev = NULL;
    char *elem;

    for (size_t i = 0; i < tableGetAmount(duplicated); i++) {
        elem = tableGet(duplicated, i);
        if (!prev || strCompare(&elem, &prev) != 0) {
            prev = elem;
            if (!phnumAdd(pnum, tableGet(duplicated, i)))
                return false;
        }
    }

    return true;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    size_t length, toReplace = 0;
    if (!pf) return NULL;
    if (!(length = isCorrect(num))) return phnumNew();

    TrieNode *longest = trieFindSeq(pf->revs, num, &toReplace);
    if (!longest) return phnumWithOne(NULL, num);

    Table *duplicated = tableNew();
    if (!duplicated ||
        !findAllRevs(longest, duplicated, num, length, toReplace)) {
        tableFree(duplicated);
        return NULL;
    }

    if (tableIsEmpty(duplicated)) {
        tableFree(duplicated);
        return phnumWithOne(NULL, num);
    }

    PhoneNumbers *pnum = phnumNew();
    if (!pnum || !tableAdd(duplicated, num) ||
        !phnumConsumeDistinct(pnum, duplicated)) {
        tableFree(duplicated);
        phnumDelete(pnum);
        return NULL;
    }

    tableFree(duplicated);
    return pnum;
}

/**
 * Zwraca liczbę numerów w strukturze. Zakłada, że @p pnum nie jest NULL-em.
 * @param pnum - wskaźnik na strukturę.
 * @return Wartość liczbowa reprezentująca liczbę numerów.
 */
static size_t phnumGetAmount(PhoneNumbers *pnum) {
    return tableGetAmount(pnum->nums);
}

PhoneNumbers *phfwdGetReverse(PhoneForward const *pf, char const *num) {
    if (!pf) return NULL;
    PhoneNumbers *revs = phfwdReverse(pf, num);
    PhoneNumbers *realRevs = phnumNew();
    if (!revs || !realRevs) {
        phnumDelete(realRevs);
        phnumDelete(revs);
        return NULL;
    }
    size_t numCount = phnumGetAmount(revs);
    PhoneNumbers *got;

    char *rev;
    for (size_t i = 0; i < numCount; i++) {
        rev = (char *) phnumGet(revs, i);
        got = phfwdGet(pf, rev);
        if (got && strcmp(phnumGet(got, 0), num) == 0)
            if (!phnumAdd(realRevs, rev)) {
                phnumDelete(realRevs);
                phnumDelete(revs);
                phnumDelete(got);
                return NULL;
            }
        phnumDelete(got);
    }

    phnumDelete(revs);
    return realRevs;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (!pnum) return;
    tableFree(pnum->nums);
    free(pnum);
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (!pnum) return NULL;
    return tableGet(pnum->nums, idx);
}
