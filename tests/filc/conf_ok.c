#include <string.h>
#include <unistd.h>
int main(void)
{
    char buf[64];
    char again[64];
    char tiny[4];
    char mark[4];
    size_t n, z, m;
    size_t (*fn)(int, char *, size_t);
    memset(buf, 0x5a, sizeof buf);
    n = confstr(_CS_PATH, buf, sizeof buf);
    if (n < 2 || n > sizeof buf || buf[n - 1] != 0 || strlen(buf) + 1 != n)
        return 2;
    z = confstr(_CS_PATH, 0, 0);
    if (z != n)
        return 3;
    memset(mark, 0x5a, sizeof mark);
    m = confstr(_CS_PATH, mark, 0);
    if (m != n || mark[0] != 0x5a)
        return 4;
    n = confstr(_CS_PATH, tiny, sizeof tiny);
    if (n != z || tiny[sizeof tiny - 1] != 0)
        return 5;
    fn = confstr;
    m = fn(_CS_PATH, again, sizeof again);
    if (m != z || strcmp(buf, again) != 0)
        return 6;
    return 0;
}
