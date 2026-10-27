#include <limits.h>
#include <string.h>

int main(void)
{
    int (*f)(int);
    int (*fl)(long);
    int (*fll)(long long);
    long hibit = (long)(1UL << 63);
    long long wide = (long long)(1ULL << 32);
    long long top = (long long)(1ULL << 63);

    if (ffs(0) != 0 || ffs(1) != 1 || ffs(2) != 2 || ffs(8) != 4)
        return 2;
    if (ffs(-1) != 1 || ffs(INT_MIN) != 32)
        return 3;
    if (ffsl(0) != 0 || ffsl(1L << 31) != 32 || ffsl(hibit) != 64)
        return 4;
    if (ffsll(0) != 0 || ffsll(wide) != 33 || ffsll(top) != 64)
        return 5;
    f = ffs;
    fl = ffsl;
    fll = ffsll;
    if (f(0) != 0 || f(8) != 4 || f(INT_MIN) != 32)
        return 6;
    if (fl(hibit) != 64 || fll(wide) != 33)
        return 7;
    return 0;
}
