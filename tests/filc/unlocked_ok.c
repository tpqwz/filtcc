#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char path[64];
    FILE *fp;
    int fd;
    int (*fg)(FILE *);
    int (*gc)(FILE *);
    int (*pw)(int, FILE *);
    int (*pc)(int, FILE *);
    int (*fl)(FILE *);
    int (*fe)(FILE *);
    int (*fr)(FILE *);
    int (*fi)(FILE *);
    void (*cl)(FILE *);
    int (*gch)(void);
    int (*pch)(int);

    snprintf(path, sizeof path, "/tmp/filc-unl-%d", (int)getpid());
    remove(path);
    fp = fopen(path, "w+");
    if (!fp)
        return 1;
    if (fputc_unlocked('a', fp) != 'a' || putc_unlocked('b', fp) != 'b')
        return 2;
    if (fflush_unlocked(fp) != 0)
        return 3;
    rewind(fp);
    if (fgetc_unlocked(fp) != 'a' || getc_unlocked(fp) != 'b')
        return 4;
    if (fgetc_unlocked(fp) != EOF || !feof_unlocked(fp) || ferror_unlocked(fp))
        return 5;
    fd = fileno_unlocked(fp);
    if (fd < 0 || fd != fileno(fp))
        return 6;
    clearerr_unlocked(fp);
    if (feof_unlocked(fp) || ferror_unlocked(fp))
        return 7;

    fg = fgetc_unlocked;
    gc = getc_unlocked;
    pw = fputc_unlocked;
    pc = putc_unlocked;
    fl = fflush_unlocked;
    fe = feof_unlocked;
    fr = ferror_unlocked;
    fi = fileno_unlocked;
    cl = clearerr_unlocked;
    rewind(fp);
    if (pw('c', fp) != 'c' || pc('d', fp) != 'd' || fl(fp) != 0)
        return 8;
    rewind(fp);
    if (fg(fp) != 'c' || gc(fp) != 'd')
        return 9;
    if (fg(fp) != EOF || !fe(fp) || fr(fp) || fi(fp) != fd)
        return 10;
    cl(fp);
    if (fe(fp))
        return 11;
    fclose(fp);

    if (!freopen(path, "w", stdout))
        return 12;
    if (putchar_unlocked('e') != 'e' || putchar_unlocked('f') != 'f')
        return 13;
    if (fflush_unlocked(stdout) != 0)
        return 14;
    if (!freopen(path, "r", stdin))
        return 15;
    if (getchar_unlocked() != 'e' || getchar_unlocked() != 'f')
        return 16;
    if (getchar_unlocked() != EOF)
        return 17;
    gch = getchar_unlocked;
    pch = putchar_unlocked;
    if (!freopen(path, "w", stdout))
        return 18;
    if (pch('g') != 'g' || fflush_unlocked(stdout) != 0)
        return 19;
    if (!freopen(path, "r", stdin))
        return 20;
    if (gch() != 'g' || gch() != EOF)
        return 21;
    fclose(stdin);
    fclose(stdout);
    remove(path);
    return 0;
}
