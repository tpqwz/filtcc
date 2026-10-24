#include <stdlib.h>
int main(void) {
    int x = 1;
    free(&x);
    return 0;
}
