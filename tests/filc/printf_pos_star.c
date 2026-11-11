#include <stdio.h>
#include <string.h>
int main(void) {
    char b[4] = {'a', 'b', 'c', 'd'};
    char buf[8];
    int n;
    n = sprintf(buf, "%2$.*1$s", 3, b);
    if (n != 3 || memcmp(buf, "abc", 3) != 0)
        return 2;
    n = sprintf(buf, "%1$.*2$s", b, 3);
    if (n != 3 || memcmp(buf, "abc", 3) != 0)
        return 3;
    return 0;
}
