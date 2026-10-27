#include <stdio.h>

int main(void)
{
    FILE *fp;
    char buf[4];
    size_t (*rd)(void *, size_t, size_t, FILE *);
    fp = tmpfile();
    if (!fp)
        return 1;
    rd = fread_unlocked;
    (void)rd(buf, 1, 8, fp);
    fclose(fp);
    return 0;
}
