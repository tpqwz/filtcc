#include <stdlib.h>
int main(void) {
    int x = 11;
    int **pp = malloc(sizeof(int *));
    int *q;
    *pp = &x;
    q = *pp;
    if (*q != 11) return 1;
    free(pp);
    return 0;
}
