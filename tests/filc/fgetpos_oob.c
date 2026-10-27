#include <stdio.h>

int main(void)
{
    FILE *fp;
    char tiny[1];
    fp = tmpfile();
    if (!fp)
        return 1;
    tiny[0] = 0;
    (void)fgetpos(fp, (fpos_t *)tiny);
    return 0;
}
