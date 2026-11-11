#include <locale.h>
#include <strings.h>

int main(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    int (*sc)(const char *, const char *, locale_t);
    char a[3];

    if (!loc)
        return 2;
    a[0] = 'A';
    a[1] = 'b';
    a[2] = 'C';
    sc = strcasecmp_l;
    (void)sc(a, "abc", loc);
    return 0;
}
