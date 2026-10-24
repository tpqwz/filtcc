#include <stdlib.h>

int main(void)
{
    char *e;
    if (setenv("FILC_IO_PAST", "ab", 1) != 0)
        return 1;
    e = getenv("FILC_IO_PAST");
    e[3] = 'z';
    return 0;
}
