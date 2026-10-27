#include <stdio.h>

int main(void)
{
    FILE *fp;
    char buf[8];
    char out[8];
    size_t (*rd)(void *, size_t, size_t, FILE *);
    size_t (*wr)(const void *, size_t, size_t, FILE *);

    fp = tmpfile();
    if (!fp)
        return 1;
    buf[0] = 'a';
    buf[1] = 'b';
    if (fwrite_unlocked(buf, 1, 2, fp) != 2)
        return 2;
    rewind(fp);
    out[0] = 0;
    out[1] = 0;
    if (fread_unlocked(out, 1, 2, fp) != 2 || out[0] != 'a' || out[1] != 'b')
        return 3;
    if (fwrite_unlocked(buf, 1, 0, fp) != 0)
        return 4;
    if (fread_unlocked(out, 0, 4, fp) != 0)
        return 5;
    rd = fread_unlocked;
    wr = fwrite_unlocked;
    rewind(fp);
    buf[0] = 'c';
    buf[1] = 'd';
    if (wr(buf, 1, 2, fp) != 2)
        return 6;
    rewind(fp);
    if (rd(out, 1, 2, fp) != 2 || out[0] != 'c' || out[1] != 'd')
        return 7;
    fclose(fp);
    return 0;
}
