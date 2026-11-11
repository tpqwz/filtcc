#include <stdio.h>
#include <string.h>
int main(void) {
    char buf[8];
    int n;
    n = sprintf(buf, "%.*s", -1, "hi");
    if (n != 2 || strcmp(buf, "hi") != 0)
        return 2;
    return 0;
}
