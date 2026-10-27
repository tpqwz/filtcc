#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char path[64];
    FILE *fp;
    int (*fg)(FILE *);
    int c;

    snprintf(path, sizeof path, "/tmp/filc-fc-%d", (int)getpid());
    fp = fopen(path, "w+");
    if (!fp)
        return 1;
    if (fputs("ab", fp) < 0)
        return 2;
    rewind(fp);
    if (fgetc(fp) != 'a')
        return 3;
    if (getc(fp) != 'b')
        return 4;
    if (fgetc(fp) != EOF)
        return 5;
    fg = fgetc;
    rewind(fp);
    if (fg(fp) != 'a')
        return 6;
    fclose(fp);
    if (!freopen(path, "r", stdin))
        return 7;
    c = getchar();
    if (c != 'a')
        return 8;
    if (getchar() != 'b' || getchar() != EOF)
        return 9;
    fclose(stdin);
    remove(path);
    return 0;
}
