#include <stdio.h>

int main(void)
{
    FILE *fp;
    char buf[4];
    fp = tmpfile();
    if (!fp)
        return 1;
    (void)fread_unlocked(buf, (size_t)-1, (size_t)-1, fp);
    fclose(fp);
    return 0;
}
