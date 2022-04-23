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

#define INIT_SIZE 64

struct PhoneForward {
    TrieNode *root;
};

struct PhoneNumbers {
    size_t amount;
    char **str;
};

static bool isValidNumber(char const *str) {
    size_t i = 0;
    while (str[i] != '\0') {
        if (str[i] < '0' || '9' < str[i]) return false;
        i++;
    }
    return true;
}

PhoneForward *phfwdNew(void) {
    PhoneForward *pf = malloc(sizeof(PhoneForward));
    if (!pf) return NULL;
    pf->root = trieNodeNew(NULL);
    if (!pf->root) return NULL;
    return pf;
}

void phfwdDelete(PhoneForward *pf) {
    trieDelete(pf->root);
    free(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    if (strcmp(num1, num2) == 0) return false;
    if (!isValidNumber(num1) || !isValidNumber(num2)) return false;

    TrieNode *v = trieInsert(&(pf->root), num1);

    if (!v) return false;
    return trieNodeSet(v, num2);
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    if (isValidNumber(num))
        trieRemoveStr(&(pf->root), num);
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
    PhoneNumbers *numbers = malloc(sizeof(PhoneNumbers));
    numbers->str = malloc(sizeof(char *) * INIT_SIZE);
    if (!numbers->str) return NULL;

    size_t charsToSubstitute;

    TrieNode *found = trieFind(pf->root, num, &charsToSubstitute);
    if (!found) {
        numbers->str[0] = (char *) num;
        return numbers;
    }

    const char *fwdPrefix = trieNodeGet(found);

    size_t newNumLength = strlen(num) + strlen(fwdPrefix) - charsToSubstitute;

    char *new = malloc(sizeof(char) * (newNumLength + 1));
    if (!new) {
        phnumDelete(numbers);
        return NULL;
    }

    size_t i = 0, j = 0;

    while (i + j < newNumLength)
        if (i < strlen(fwdPrefix)) {
            new[i] = fwdPrefix[i];
            i++;
        }
        else {
            new[strlen(fwdPrefix) + j] = num[j + charsToSubstitute];
            j++;
        }

    numbers->str[0] = new;

    return numbers;
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
    return NULL;
}

void phnumDelete(PhoneNumbers *pnum) {
    if (!pnum) {
        for (size_t i = 0; i < pnum->amount; i++)
            free(pnum->str[i]);
        free(pnum->str);
        free(pnum);
    }
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
    if (!pnum || idx > pnum->amount) return NULL;
    return pnum->str[idx];
}
