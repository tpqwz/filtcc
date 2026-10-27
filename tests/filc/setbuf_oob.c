#include <stdio.h>

int main(void)
{
    FILE *fp;
    char tiny[8];
    fp = tmpfile();
    if (!fp)
        return 1;
    setbuf(fp, tiny);
    fclose(fp);
    return 0;
}
