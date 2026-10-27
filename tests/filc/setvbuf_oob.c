#include <stdio.h>

int main(void)
{
    FILE *fp;
    char tiny[4];
    fp = tmpfile();
    if (!fp)
        return 1;
    setvbuf(fp, tiny, _IOFBF, 64);
    fclose(fp);
    return 0;
}
