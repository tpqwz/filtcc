#include <stdio.h>
#include <string.h>

int main(void)
{
    unsigned major, minor, patch;
    unsigned big[2];
    int v1, v2, v3, n, x;
    unsigned long long count;
    unsigned long a, b;
    char c;
    char ver[] = "xx1.2.3";
    char tail[] = "10.20.30tail";
    char line[] = "42Z";
    char map[] = "7fff-8abc";
    char word[4];
    int (*fn)(const char *, const char *, ...);

    if (sscanf(ver + 2, "%u.%u.%u", &major, &minor, &patch) != 3)
        return 1;
    if (major != 1 || minor != 2 || patch != 3)
        return 2;

    if (sscanf(tail, "%d.%d.%d", &v1, &v2, &v3) != 3)
        return 3;
    if (v1 != 10 || v2 != 20 || v3 != 30)
        return 4;

    if (sscanf(line, "%llu%c", &count, &c) != 2 || count != 42 || c != 'Z')
        return 5;

    if (sscanf(map, "%lx-%lx", &a, &b) != 2 || a != 0x7fffUL || b != 0x8abcUL)
        return 6;

    /* The destination is interior to big. Four bytes still fit. */
    if (sscanf("7", "%u", &big[1]) != 1 || big[1] != 7)
        return 7;

    n = 0;
    if (sscanf("12", "%d%n", &v1, &n) != 1 || v1 != 12 || n != 2)
        return 8;

    if (sscanf("hello", "%3s", word) != 1 || strcmp(word, "hel") != 0)
        return 9;

    if (sscanf("z", "%d", &v1) != 0)
        return 10;

    fn = sscanf;
    if (fn("9", "%d", &x) != 1 || x != 9)
        return 11;
    return 0;
}
