#include <stdio.h>

int main(void)
{
    FILE *fp;
    fp = tmpfile();
    if (!fp)
        return 1;
    (void)fread_unlocked((char *)"0123456789abcdef", 1, 8, fp);
    fclose(fp);
    return 0;
}
