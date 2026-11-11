#include <locale.h>
#include <strings.h>

int main(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    char b[3];

    if (!loc)
        return 2;
    b[0] = 'A';
    b[1] = 'b';
    b[2] = 'C';
    (void)strcasecmp_l("abc", b, loc);
    return 0;
}
