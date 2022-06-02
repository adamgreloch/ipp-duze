#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dynamic_table.h"
#include "alphabet.h"

#define INIT_SIZE 64 /**< Początkowy rozmiar dynamicznej tablicy @p
                          Table#data */

struct Table {
    size_t amount; /**< Liczba numerów telefonów przechowywanych w @p str. */
    char **data; /**< Dynamiczna tablica o początkowym rozmiarze @p INIT_SIZE,
                     w której przechowywane są poprawne ciągi znaków. */
    size_t size; /**< Rozmiar tablicy. */
};

Table *tableNew() {
    Table *t = malloc(sizeof(Table));
    if (!t) return NULL;

    t->data = calloc(INIT_SIZE, sizeof(char *));
    if (!t->data) {
        free(t);
        return NULL;
    }

    t->size = INIT_SIZE;
    t->amount = 0;

    return t;
}

void tableSort(Table *t, int (*cmp)(const void *, const void *)) {
    if (!t || !t->data) return;
    qsort(t->data, t->amount, sizeof(char *), cmp);
}

static bool tableResize(Table *t) {
    if (t->amount == t->size) {
        t->size *= 2;
        char **backup = t->data;
        t->data = realloc(t->data, t->size * sizeof(char *));
        if (!t->data) {
            t->data = backup;
            return false;
        }
    }
    return true;
}

bool tableAdd(Table *t, const char *str) {
    if (!tableResize(t)) return false;

    t->data[t->amount] = malloc((isCorrect(str) + 1) * sizeof(char));
    if (!t->data[t->amount])
        return false;
    strcpy(t->data[t->amount], str);
    t->amount++;

    return true;
}

bool tableAddPtr(Table *t, char *str) {
    if (!tableResize(t)) return false;

    t->data[t->amount++] = str;

    return true;
}

void tableDelete(Table *t) {
    if (!t) return;
    for (size_t i = 0; i < t->amount; i++)
        free(t->data[i]);
    free(t->data);
    free(t);
}

char *tableGet(Table *t, size_t idx) {
    if (!t || idx > t->amount) return NULL;
    return t->data[idx];
}

size_t tableGetAmount(Table *t) {
    if (!t) return -1;
    return t->amount;
}
