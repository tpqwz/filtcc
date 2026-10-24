#include <stdatomic.h>
int main(void)
{
    int a = 11, b = 22, c = 33;
    int *slot = &a;
    int *loaded, *old, *exp;
    int ok, v;

    __atomic_store_n(&slot, &b, __ATOMIC_SEQ_CST);
    loaded = __atomic_load_n(&slot, __ATOMIC_SEQ_CST);
    if (loaded != &b || *loaded != 22)
        return 1;

    old = atomic_exchange(&slot, &c);
    if (old != &b || *old != 22)
        return 2;
    if (slot != &c || *slot != 33)
        return 3;

    exp = &c;
    ok = __atomic_compare_exchange_n(&slot, &exp, &a, 0,
                                    __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if (!ok || slot != &a || *slot != 11)
        return 4;
    if (exp != &c || *exp != 33)
        return 5;

    exp = &b;
    ok = __atomic_compare_exchange_n(&slot, &exp, &c, 0,
                                    __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    if (ok)
        return 6;
    if (slot != &a || *slot != 11)
        return 7;
    if (exp != &a || *exp != 11)
        return 8;

    /* The atomic address can be a pointer variable, not only &local. */
    {
        int **pp = &slot;
        int *got;
        __atomic_store_n(pp, &b, __ATOMIC_SEQ_CST);
        got = __atomic_load_n(pp, __ATOMIC_SEQ_CST);
        if (got != &b || *got != 22 || slot != &b)
            return 12;
    }

    v = 1;
    __atomic_store_n(&v, 5, __ATOMIC_SEQ_CST);
    if (__atomic_load_n(&v, __ATOMIC_SEQ_CST) != 5)
        return 9;
    if (__atomic_fetch_add(&v, 3, __ATOMIC_SEQ_CST) != 5)
        return 10;
    if (v != 8)
        return 11;
    return 0;
}
