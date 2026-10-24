#include <stdlib.h>

static int nest_n;

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

static int cmp_char(const void *a, const void *b)
{
    unsigned char x = *(const unsigned char *)a;
    unsigned char y = *(const unsigned char *)b;
    return (x > y) - (x < y);
}

static int cmp_ptr(const void *a, const void *b)
{
    const int *x = *(const int *const *)a;
    const int *y = *(const int *const *)b;
    int xv = *x;
    int yv = *y;
    return (xv > yv) - (xv < yv);
}

struct item {
    int k;
    int *p;
};

static int cmp_item(const void *a, const void *b)
{
    const struct item *x = a;
    const struct item *y = b;
    int xv = *x->p;
    int yv = *y->p;
    return (xv > yv) - (xv < yv);
}

static int cmp_nest(const void *a, const void *b)
{
    int small[3];
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (nest_n < 2) {
        nest_n++;
        small[0] = 3;
        small[1] = 1;
        small[2] = 2;
        qsort(small, 3, sizeof(int), cmp_int);
        if (small[0] != 1 || small[1] != 2 || small[2] != 3)
            nest_n = 100;
    }
    return (x > y) - (x < y);
}

int main(void)
{
    int dups[5] = {2, 2, 1, 2, 1};
    int big[64];
    int vals[4] = {40, 10, 30, 20};
    int *ptrs[4];
    int svals[3] = {30, 10, 20};
    struct item items[3];
    int one = 7;
    int sorted[4] = {1, 2, 3, 4};
    int key, miss, i;
    int *found;
    char letters[5] = {'e', 'a', 'c', 'b', 'd'};
    static const int cro[4] = {1, 2, 3, 4};
    const int *cf;
    void (*qs)(void *, size_t, size_t, int (*)(const void *, const void *));
    void *(*bs)(const void *, const void *, size_t, size_t,
        int (*)(const void *, const void *));
    int ind[3] = {3, 1, 2};

    qsort(NULL, 0, sizeof(int), NULL);
    qsort(&one, 1, sizeof one, cmp_int);
    if (one != 7)
        return 1;

    qsort(dups, 5, sizeof(int), cmp_int);
    if (dups[0] != 1 || dups[1] != 1 || dups[2] != 2 || dups[3] != 2 || dups[4] != 2)
        return 2;

    for (i = 0; i < 64; i++)
        big[i] = 63 - i;
    qsort(big, 64, sizeof(int), cmp_int);
    for (i = 0; i < 64; i++) {
        if (big[i] != i)
            return 3;
    }

    ptrs[0] = &vals[0];
    ptrs[1] = &vals[1];
    ptrs[2] = &vals[2];
    ptrs[3] = &vals[3];
    qsort(ptrs, 4, sizeof(int *), cmp_ptr);
    if (ptrs[0] != &vals[1] || *ptrs[0] != 10)
        return 4;
    if (ptrs[1] != &vals[3] || *ptrs[1] != 20)
        return 4;
    if (ptrs[2] != &vals[2] || *ptrs[2] != 30)
        return 4;
    if (ptrs[3] != &vals[0] || *ptrs[3] != 40)
        return 4;

    items[0].k = 3;
    items[0].p = &svals[0];
    items[1].k = 1;
    items[1].p = &svals[1];
    items[2].k = 2;
    items[2].p = &svals[2];
    qsort(items, 3, sizeof items[0], cmp_item);
    if (items[0].k != 1 || items[0].p != &svals[1] || *items[0].p != 10)
        return 5;
    if (items[1].k != 2 || items[1].p != &svals[2] || *items[1].p != 20)
        return 5;
    if (items[2].k != 3 || items[2].p != &svals[0] || *items[2].p != 30)
        return 5;

    key = 3;
    found = bsearch(&key, sorted, 4, sizeof(int), cmp_int);
    if (!found || *found != 3 || found[-1] != 2)
        return 6;
    miss = 99;
    if (bsearch(&miss, sorted, 4, sizeof(int), cmp_int) != NULL)
        return 7;

    qsort(letters, 5, 1, cmp_char);
    if (letters[0] != 'a' || letters[1] != 'b' || letters[2] != 'c'
        || letters[3] != 'd' || letters[4] != 'e')
        return 8;

    key = 3;
    cf = bsearch(&key, cro, 4, sizeof(int), cmp_int);
    if (!cf || *cf != 3 || cf[-1] != 2)
        return 9;

    nest_n = 0;
    qsort(dups, 5, sizeof(int), cmp_nest);
    if (nest_n == 100 || nest_n < 1)
        return 10;
    if (dups[0] != 1 || dups[4] != 2)
        return 10;

    qs = qsort;
    qs(ind, 3, sizeof(int), cmp_int);
    if (ind[0] != 1 || ind[1] != 2 || ind[2] != 3)
        return 11;
    bs = bsearch;
    key = 2;
    found = bs(&key, ind, 3, sizeof(int), cmp_int);
    if (!found || *found != 2)
        return 12;
    return 0;
}
