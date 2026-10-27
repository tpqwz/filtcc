#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static FILE *make(char *path, const char *text)
{
    FILE *fp;
    snprintf(path, 64, "/tmp/filc-gl-%d", (int)getpid());
    fp = fopen(path, "w+");
    if (!fp)
        return NULL;
    if (fputs(text, fp) < 0)
        return NULL;
    rewind(fp);
    return fp;
}

int main(void)
{
    char path[64];
    char buf[32];
    char *p, *q;
    size_t n;
    ssize_t r;
    FILE *fp;
    ssize_t (*fn)(char **, size_t *, FILE *);
    ssize_t (*fd)(char **, size_t *, int, FILE *);

    fp = make(path, "ab\ncd");
    if (!fp)
        return 1;
    p = NULL;
    n = 99;
    r = getline(&p, &n, fp);
    if (r != 3 || !p || strcmp(p, "ab\n") != 0 || n < 4)
        return 2;
    q = p;
    r = getline(&p, &n, fp);
    if (r != 2 || p != q || strcmp(p, "cd") != 0)
        return 3;
    r = getline(&p, &n, fp);
    if (r != -1 || p != q)
        return 4;
    free(p);
    fclose(fp);
    remove(path);

    fp = make(path, "");
    if (!fp)
        return 5;
    p = NULL;
    n = 0;
    r = getline(&p, &n, fp);
    /* Immediate EOF still allocates. glibc does not promise a NUL. */
    if (r != -1 || !p || n == 0)
        return 6;
    p[0] = 0;
    p[n - 1] = 0;
    free(p);
    fclose(fp);
    remove(path);

    fp = make(path, "ab\n");
    if (!fp)
        return 7;
    memset(buf, 0, sizeof buf);
    p = buf;
    n = sizeof buf;
    r = getline(&p, &n, fp);
    if (r != 3 || p != buf || n != sizeof buf || strcmp(buf, "ab\n") != 0)
        return 8;
    p = buf + 4;
    n = 16;
    rewind(fp);
    r = getline(&p, &n, fp);
    if (r != 3 || p != buf + 4 || strcmp(p, "ab\n") != 0 || buf[3] != 0)
        return 9;
    fclose(fp);
    remove(path);

    fp = make(path, "hello\n");
    if (!fp)
        return 10;
    p = malloc(4);
    if (!p)
        return 11;
    n = 4;
    q = p;
    r = getline(&p, &n, fp);
    if (r != 6 || !p || p == q || strcmp(p, "hello\n") != 0 || n < 7)
        return 12;
    free(p);
    fclose(fp);
    remove(path);

    fp = make(path, "aaXbb\n");
    if (!fp)
        return 13;
    p = NULL;
    n = 0;
    r = getdelim(&p, &n, 'X', fp);
    if (r != 3 || !p || strcmp(p, "aaX") != 0)
        return 14;
    free(p);
    fclose(fp);
    remove(path);

    fp = make(path, "xy\n");
    if (!fp)
        return 15;
    p = NULL;
    n = 0;
    fn = getline;
    r = fn(&p, &n, fp);
    if (r != 3 || !p || strcmp(p, "xy\n") != 0)
        return 16;
    free(p);
    rewind(fp);
    p = NULL;
    n = 0;
    fd = getdelim;
    r = fd(&p, &n, '\n', fp);
    if (r != 3 || !p || strcmp(p, "xy\n") != 0)
        return 17;
    free(p);
    fclose(fp);
    remove(path);
    return 0;
}
