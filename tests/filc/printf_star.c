#include <stdio.h>
int main(void) {
    char b[4] = {'a', 'b', 'c', 'd'};
    if (printf("%.*s", 3, b) != 3)
        return 2;
    return 0;
}
