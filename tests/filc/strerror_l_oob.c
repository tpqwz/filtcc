#include <locale.h>
#include <string.h>

int main(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    char *s;
    volatile char c;
    size_t n = 0;

    if (!loc)
        return 2;
    s = strerror_l(0, loc);
    while (s[n])
        n++;
    c = s[n + 1];
    (void)c;
    return 0;
}
