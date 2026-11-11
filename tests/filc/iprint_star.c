#include <stdio.h>
#include <string.h>
int main(void) {
    int (*sp)(char *, const char *, ...) = sprintf;
    char buf[16];
    char b[4] = {'a', 'b', 'c', 'd'};
    int n;
    n = sp(buf, "%.*s", 3, b);
    if (n != 3 || memcmp(buf, "abc", 3) != 0)
        return 2;
    n = sp(buf, "%2$s", "no", "yes");
    if (n != 3 || strcmp(buf, "yes") != 0)
        return 3;
    return 0;
}
