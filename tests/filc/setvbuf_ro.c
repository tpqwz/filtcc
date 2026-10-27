#include <stdio.h>

int main(void)
{
    FILE *fp;
    fp = tmpfile();
    if (!fp)
        return 1;
    setvbuf(fp, (char *)"0123456789abcdef", _IOFBF, 16);
    fclose(fp);
    return 0;
}
