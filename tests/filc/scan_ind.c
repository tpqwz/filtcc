#include <string.h>

int main(void)
{
    char *(*cp)(char *, const char *) = stpcpy;
    size_t (*sp)(const char *, const char *) = strspn;
    char *(*pb)(const char *, const char *) = strpbrk;
    void *(*mc)(void *, const void *, int, unsigned long) = memccpy;
    char dst[16];
    char *end;
    char *hit;

    end = cp(dst, "ok");
    if (end != dst + 2 || dst[0] != 'o' || dst[2] != 0)
        return 1;
    if (sp("12ab", "21") != 2)
        return 2;
    hit = pb("abX", "X");
    if (!hit || *hit != 'X')
        return 3;
    memset(dst, 0, sizeof dst);
    end = mc(dst, "abY", 'Y', 3);
    if (!end || end[-1] != 'Y' || dst[0] != 'a')
        return 4;
    return 0;
}
