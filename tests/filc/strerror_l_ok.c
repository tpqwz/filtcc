#include <errno.h>
#include <locale.h>
#include <string.h>

static int walk(const char *s)
{
    if (!s)
        return 0;
    while (*s)
        s++;
    return 1;
}

int main(void)
{
    locale_t loc;
    char *(*f)(int, locale_t);
    const char *s, *t;

    loc = newlocale(LC_ALL_MASK, "C", (locale_t)0);
    if (!loc)
        return 2;
    s = strerror_l(0, loc);
    if (!walk(s) || strcmp(s, "Success") != 0)
        return 3;
    t = strerror_l(ENOENT, loc);
    if (!walk(t) || strcmp(t, "No such file or directory") != 0)
        return 4;
    if (s == t)
        return 5;
    s = strerror_l(99999, loc);
    if (!walk(s) || strcmp(s, "Unknown error 99999") != 0)
        return 6;
    t = strerror_l(ENOENT, LC_GLOBAL_LOCALE);
    if (!walk(t) || strcmp(t, "No such file or directory") != 0)
        return 7;
    f = strerror_l;
    s = f(0, loc);
    if (!walk(s) || strcmp(s, "Success") != 0)
        return 8;
    s = f(ENOENT, LC_GLOBAL_LOCALE);
    if (!walk(s) || strcmp(s, "No such file or directory") != 0)
        return 9;
    freelocale(loc);
    return 0;
}
