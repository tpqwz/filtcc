#include <locale.h>
#include <strings.h>

int main(void)
{
    locale_t loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);

    if (!loc)
        return 2;
    (void)strcasecmp_l(0, "a", loc);
    return 0;
}
