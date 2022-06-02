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

