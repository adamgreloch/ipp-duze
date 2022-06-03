/** @file
 * Implementacja klasy definiującej alfabet.
 *
 * \e Alfabetem nazywamy znaki ze zbioru \f$\Omega=\{0,1,\ldots,9\}\cup\{*,
 * \#\}\f$.
 * Niepuste ciągi znaków z alfabetu nazywamy odtąd <em> poprawnymi ciągami
 * znaków </em>.
 *
 * @author Adam Greloch <ag438473@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>
#include <stdbool.h>
#include "alphabet.h"

int getValue(char c) {
    if (c != '*' && c != '#' && (c < '0' || '9' < c)) return -1;

    switch (c) {
        case '*':
            return 10;
        case '#':
            return 11;
        default:
            return c - '0';
    }
}

size_t isCorrect(char const *str) {
    if (!str) return false;
    size_t i = 0;
    while (str[i] != '\0') {
        if (getValue(str[i]) == -1)
            return 0;
        i++;
    }
    return i;
}

int strCompare(const void *a, const void *b) {
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

