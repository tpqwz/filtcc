#include <stdlib.h>
void filc_collect(void);
int main(void) {
    int *p = malloc(sizeof(int));
    *p = 1;
    free(p);
    filc_collect();
    return *p;
}
