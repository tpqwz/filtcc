#include <stdio.h>
int main(void) {
    int (*sp)(char *, const char *, ...) = sprintf;
    char buf[8];
    char b[4] = {'a', 'b', 'c', 'd'};
    (void)sp(buf, "%.*s", 5, b);
    return 0;
}
