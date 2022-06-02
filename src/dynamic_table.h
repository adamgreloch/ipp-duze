#ifndef __DYNAMIC_TABLE_H__
#define __DYNAMIC_TABLE_H__

struct Table;

typedef struct Table Table;

Table *tableNew();

void tableSort(Table *t, int (*cmp)(const void *, const void *));

bool tableAdd(Table *t, const char *str);

bool tableAddPtr(Table *t, char *str);

char *tableGet(Table *t, size_t idx);

void tableDelete(Table *t);

size_t tableGetAmount(Table *t);

#endif /* __DYNAMIC_TABLE_H__ */
