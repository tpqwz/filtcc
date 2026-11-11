#include <locale.h>

extern long strtol_l(const char *, char **, int, locale_t);

int main(void)
{
    locale_t loc;
    char b[3];

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    b[0] = '1';
    b[1] = '2';
    b[2] = '3';
    (void)strtol_l(b, 0, 10, loc);
    freelocale(loc);
    return 0;
}
