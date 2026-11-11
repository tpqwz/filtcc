#include <stdio.h>
#include <string.h>
int main(void) {
    char buf[16];
    int n;
    n = sprintf(buf, "%2$s %1$d", 7, "ok");
    if (n != 4 || strcmp(buf, "ok 7") != 0)
        return 2;
    return 0;
}
