#include <errno.h>
#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    char *s;

    if (!loc)
        return 2;
    s = strerror_l(ENOENT, loc);
    s[0] = 'X';
    return 0;
}
