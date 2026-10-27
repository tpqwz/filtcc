#include <stdio.h>

int main(void)
{
    FILE *fp;
    char tiny[1];
    fp = tmpfile();
    if (!fp)
        return 1;
    tiny[0] = 0;
    (void)fsetpos(fp, (const fpos_t *)tiny);
    return 0;
}
