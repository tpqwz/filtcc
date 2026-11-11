#include <locale.h>

extern long strtol_l(const char *, char **, int, locale_t);

static char *const roend = 0;

int main(void)
{
    locale_t loc;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    (void)strtol_l("12", (char **)&roend, 10, loc);
    freelocale(loc);
    return 0;
}
