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

/** @brief Struktura przechowująca przekierowania telefonów.
 * Struktura przechowująca przekierowania telefonów trzyma je w postaci
 * węzłów w drzewie trie. Prefiksy zaczynają się w korzeniu drzewa i kończą w
 * odpowiednich węzłach. Jeśli dla danego prefiksu ustalono przekierowanie,
 * to wartością w węźle jest ciąg znaków go reprezentujący.
 * @see trie.h
 */
struct PhoneForward {
    TrieNode *root; /**< Wskaźnik na korzeń struktury. */
};

#define INIT_SIZE 64 /**< Początkowy rozmiar dynamicznej tablicy @p
                          PhoneNumbers#nums */

/**
 * Struktura przechowująca przekierowania numerów telefonów.
 */
struct PhoneNumbers {
    size_t amount; /**< Liczba numerów telefonów przechowywanych w @p str. */
    char **nums; /**< Dynamiczna tablica o początkowym rozmiarze @p INIT_SIZE,
                     w której przechowywane są numery telefonów. */
    size_t size; /**< Rozmiar tablicy. */
};

/**
 * @brief Sprawdza, czy @p str jest numerem telefonu.
 * Sprawdza, czy ciąg znaków @p str reprezentuje numer telefonu tzn.
 * czy jest ciągiem złożonym wyłącznie z cyfr \f$0,1,...,9\f$. Liczy jego
 * długość.
 * @param[in] str - sprawdzany ciąg znaków.
 * @return Dodatnia wartość liczbowa, jeśli @p str jest prawidłowy lub
 *         zero przeciwnym wypadku.
 */
static size_t isNumber(char const *str) {
    if (!str) return false;

    size_t i = 0;
    while (str[i] != '\0') {
        if (str[i] < '0' || '9' < str[i]) return 0;
        i++;
    }

    return i;
}

PhoneForward *phfwdNew(void) {
    PhoneForward *pf = malloc(sizeof(PhoneForward));
    if (!pf) return NULL;
    pf->root = trieNodeNew(NULL);
    if (!pf->root) return NULL;
    return pf;
}

void phfwdDelete(PhoneForward *pf) {
    if (!pf) return;
    trieDelete(pf->root);
    free(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if (!pf) return false;
    size_t length;
    if (!isNumber(num1) || !(length = isNumber(num2))) return false;
    if (strcmp(num1, num2) == 0) return false;

    TrieNode *v = trieInsertStr(&(pf->root), num1);

    if (!v) return false;
    return trieNodeSet(v, num2, length);
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if (pf && isNumber(num))
        trieRemoveStr(&(pf->root), num);
}

/**
 * @brief Alokuje nową strukturę @p PhoneNumbers.
 * @return Wskaźnik na strukturę @p PhoneNumbers lub NULL, gdy nie udało się
 *         alokować pamięci.
 */
static PhoneNumbers *pnumNew() {
    PhoneNumbers *numbers = malloc(sizeof(PhoneNumbers));
    if (!numbers) return NULL;

    numbers->nums = calloc(INIT_SIZE, sizeof(char *));
    if (!numbers->nums) return NULL;

    numbers->size = INIT_SIZE;
    numbers->amount = 0;

    return numbers;
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    if (!pf) return NULL;

    PhoneNumbers *numbers = pnumNew();
    if (!numbers) return NULL;

    size_t numLength = isNumber(num);
    if (!numLength) return numbers;

    size_t toReplace;

    TrieNode *found = trieFind(pf->root, num, &toReplace);
    if (!found) {
        numbers->nums[0] = (char *) num;
        return numbers;
    }

    const char *fwdPrefix = trieNodeGet(found);

    size_t fwdPrefixLength = strlen(fwdPrefix);
    size_t newNumLength = numLength + fwdPrefixLength - toReplace;

    char *new = calloc((newNumLength + 1), sizeof(char));
    if (!new) {
        phnumDelete(numbers);
        return NULL;
    }

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

    numbers->nums[0] = new;
    numbers->amount++;

    return numbers;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    if (!pf || !num) return NULL;
    return NULL;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (!pnum) return;
    for (size_t i = 0; i < pnum->amount; i++)
        free(pnum->nums[i]);
    free(pnum->nums);
    free(pnum);
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (!pnum || idx > pnum->amount) return NULL;
    return pnum->nums[idx];
}
