#include <stdio.h>
int main(void) {
    char b[4] = {'a', 'b', 'c', 'd'};
    (void)printf("%2$s\n", 1, b);
    return 0;
}
