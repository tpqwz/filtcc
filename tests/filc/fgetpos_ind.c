#include <stdio.h>

int main(void)
{
    FILE *fp;
    int (*gp)(FILE *, fpos_t *);
    char tiny[1];
    fp = tmpfile();
    if (!fp)
        return 1;
    tiny[0] = 0;
    gp = fgetpos;
    (void)gp(fp, (fpos_t *)tiny);
    return 0;
}
