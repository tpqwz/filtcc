#include <locale.h>

extern long strtol_l(const char *, char **, int, locale_t);

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)strtol_l(0, 0, 10, loc);
    freelocale(loc);
    return 0;
}
