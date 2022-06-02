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
