#include <string.h>
#include <strings.h>

/* bzero is a libc symbol even when the header hides the prototype. */
void bzero(void *s, unsigned long n);

int main(void)
{
    char dst[16];
    char src[4] = {'a', 'b', 'c', 'd'};
    char *end;
    char *hit;
    unsigned long slot;
    int x = 7;
    int *p = &x;
    int *q;
    unsigned char *b;
    unsigned char *bits;
    int i;

    if (strspn("abc123", "cba") != 3)
        return 1;
    if (strspn("abc", "abc") != 3)
        return 2;
    if (strspn("", "abc") != 0)
        return 3;
    if (strcspn("abc123", "0123456789") != 3)
        return 4;
    if (strcspn("abc", "xyz") != 3)
        return 5;
    if (strcspn("", "a") != 0)
        return 6;
    hit = strpbrk("abc123", "321");
    if (!hit || *hit != '1' || hit[1] != '2')
        return 7;
    if (strpbrk("abc", "xyz") != 0)
        return 8;
    if (strpbrk("", "a") != 0)
        return 9;

    end = stpcpy(dst, "hi");
    if (end != dst + 2 || *end != 0 || strcmp(dst, "hi") != 0)
        return 10;
    end = stpncpy(dst, "hello", 8);
    if (end != dst + 5 || strcmp(dst, "hello") != 0)
        return 11;
    end = stpncpy(dst, src, 4);
    if (end != dst + 4 || memcmp(dst, "abcd", 4) != 0)
        return 12;
    end = stpncpy(dst, "z", 0);
    if (end != dst)
        return 13;

    memset(dst, 0x5a, sizeof dst);
    end = memccpy(dst, "abXcd", 'X', 5);
    if (!end || end[-1] != 'X' || memcmp(dst, "abX", 3) != 0)
        return 14;
    if (memccpy(dst, "abcd", 'Z', 4) != 0)
        return 15;
    if (memcmp(dst, "abcd", 4) != 0)
        return 16;
    if (memccpy(dst, "abcd", 'a', 0) != 0)
        return 17;

    memset(dst, 0x5a, sizeof dst);
    explicit_bzero(dst, 8);
    for (i = 0; i < 8; i++)
        if (dst[i] != 0)
            return 18;
    memset(dst, 0x5a, sizeof dst);
    bzero(dst, 4);
    if (dst[0] || dst[3] || dst[4] != 0x5a)
        return 19;

    /* Zeroing the integer bits leaves the capability in place. */
    memcpy(&slot, &p, sizeof p);
    explicit_bzero(&slot, sizeof slot);
    b = (unsigned char *)&slot;
    bits = (unsigned char *)&p;
    for (i = 0; i < (int)sizeof p; i++)
        b[i] = bits[i];
    q = *(int **)&slot;
    if (q != &x || *q != 7)
        return 20;
    return 0;
}
