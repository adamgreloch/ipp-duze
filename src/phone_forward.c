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
    TrieNode *fwds; /**< Wskaźnik na korzeń struktury przechowującej jako węzły prefiksy
                         dla których ustalono przekierowanie. */
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
static char *replacePrefix(char const *num, char const *fwdPrefix,
                           size_t numLength, size_t toReplace) {
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

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    if (!pf) return NULL;

    PhoneNumbers *pnum = phnumNew();
    if (!pnum) return NULL;

    size_t length = isCorrect(num);
    if (!length) return pnum;

    size_t toReplace;
    TrieNode *found = trieFindSeq(pf->fwds, num, &toReplace);
    if (!found) {
        phnumAdd(pnum, num);
        return pnum;
    }

    char *replaced = replacePrefix(num, trieNodeGetSeq(found), length,
                                   toReplace);
    if (!replaced) {
        phnumDelete(pnum);
        return NULL;
    }

    phnumAdd(pnum, replaced);
    free(replaced);

    return pnum;
}

static int numCompare(const void *a, const void *b) {
    char *num1 = *(char **) a;
    char *num2 = *(char **) b;
    size_t pos = 0;

    while (true) {
        if (num1[pos] == '\0' && num2[pos] == '\0')
            return 0;
        else if (num1[pos] == '\0')
            return -1;
        else if (num2[pos] == '\0')
            return 1;
        else if (num1[pos] == num2[pos])
            pos++;
        else
            return getValue(num1[pos]) - getValue(num2[pos]);
    }
}

static Table *findAllRevs(TrieNode *from, char const *num,
                          size_t length, size_t toReplace) {
    Table *revs = tableNew();

    TrieNode *curr = from;
    char **arr = NULL;
    size_t size;

    while (curr) {
        free(arr);
        arr = listToArray(trieGetList(curr), &size);
        if (arr) {
            qsort(arr, size, sizeof(char *), numCompare);
            for (size_t i = 0; i < size; i++)
                tableAddPtr(revs,
                            replacePrefix(num, arr[i], length, toReplace));
        }
        curr = trieGetParent(curr);
        toReplace--;
    }

    return revs;
}

static void phnumAddDistinct(PhoneNumbers *pnum, Table *duplicated) {
    tableSort(duplicated, numCompare);

    char *prev = NULL;
    char *elem;

    for (size_t i = 0; i < tableGetAmount(duplicated); i++) {
        elem = tableGet(duplicated, i);
        if (!prev || numCompare(&elem, &prev) != 0) {
            prev = elem;
            phnumAdd(pnum, tableGet(duplicated, i));
        }
    }
}

// TODO doc-update: na podstawie forum
PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    size_t length, toReplace = 0;
    if (!pf) return NULL;

    PhoneNumbers *pnum = phnumNew();

    if (!(length = isCorrect(num))) {
        return pnum;
    }

    TrieNode *longest = trieFindSeq(pf->revs, num, &toReplace);
    if (!longest) {
        phnumAdd(pnum, num);
        return pnum;
    }

    Table *duplicated = findAllRevs(longest, num, length, toReplace);
    tableAdd(duplicated, num);
    phnumAddDistinct(pnum, duplicated);

    return pnum;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (!pnum) return;
    tableDelete(pnum->nums);
    free(pnum);
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (!pnum) return NULL;
    return tableGet(pnum->nums, idx);
}
