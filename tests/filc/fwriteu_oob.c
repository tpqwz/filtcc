#include <stdio.h>

int main(void)
{
    FILE *fp;
    char buf[4];
    fp = tmpfile();
    if (!fp)
        return 1;
    (void)fwrite_unlocked(buf, 1, 8, fp);
    fclose(fp);
    return 0;
}
