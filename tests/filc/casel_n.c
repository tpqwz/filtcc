#include <locale.h>
#include <strings.h>

int main(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    char a[2];

    if (!loc)
        return 2;
    a[0] = 'A';
    a[1] = 'B';
    (void)strncasecmp_l(a, "ab", 3, loc);
    return 0;
}
