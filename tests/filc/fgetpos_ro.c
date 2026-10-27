#include <stdio.h>

int main(void)
{
    FILE *fp;
    fp = tmpfile();
    if (!fp)
        return 1;
    (void)fgetpos(fp, (fpos_t *)"0123456789abcdef");
    return 0;
}
