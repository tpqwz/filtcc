#include <string.h>
#include <unistd.h>
int main(void)
{
    char buf[256];
    char again[256];
    int (*fn)(char *, size_t);
    int n;
    memset(buf, 0x5a, sizeof buf);
    n = getdomainname(buf, sizeof buf);
    if (n != 0 || buf[sizeof buf - 1] != 0x5a)
        return 2;
    if (strlen(buf) >= sizeof buf)
        return 3;
    memset(again, 0x5a, sizeof again);
    n = getdomainname(again, 0);
    if (n != 0 || again[0] != 0x5a)
        return 4;
    fn = getdomainname;
    n = fn(again, sizeof again);
    if (n != 0 || strcmp(buf, again) != 0)
        return 5;
    return 0;
}
