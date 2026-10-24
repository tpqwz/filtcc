#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
    char a[] = "AbC";
    char hay[] = "xxAbCyy";
    char blob[] = "hello-FILC-world";
    char *p;
    void *m;
    struct stat st;
    int fd;
    int (*sc)(const char *, const char *);
    void *(*mm)(const void *, size_t, const void *, size_t);
    int (*stf)(const char *, struct stat *);
    const char *path = "/tmp/filc-stat-probe";

    if (strcasecmp(a, "abc") != 0)
        return 1;
    if (strcasecmp("AbC", "abc") != 0)
        return 2;
    if (strncasecmp(a, "abZ", 2) != 0)
        return 3;
    if (strncasecmp(a, "abZ", 3) == 0)
        return 4;
    p = strcasestr(hay, "abc");
    if (!p || p != hay + 2 || p[-1] != 'x' || p[0] != 'A')
        return 5;
    if (strcasestr(hay, "nope") != NULL)
        return 6;
    m = memmem(blob, sizeof blob - 1, "FILC", 4);
    if (!m || m != (void *)(blob + 6) || ((char *)m)[-1] != '-' || ((char *)m)[0] != 'F')
        return 7;
    if (memmem(blob, sizeof blob - 1, "nope", 4) != NULL)
        return 8;
    if (memmem(blob, sizeof blob - 1, "x", 0) != (void *)blob)
        return 9;

    fd = open(path, O_CREAT | O_TRUNC | O_RDWR, 0600);
    if (fd < 0)
        return 10;
    if (write(fd, "hi", 2) != 2)
        return 11;
    if (stat(path, &st) != 0 || st.st_size != 2)
        return 12;
    if (fstat(fd, &st) != 0 || st.st_size != 2)
        return 13;
    if (lstat(path, &st) != 0 || st.st_size != 2)
        return 14;
    close(fd);
    unlink(path);

    sc = strcasecmp;
    if (sc("Hi", "hi") != 0)
        return 15;
    mm = memmem;
    m = mm(blob, sizeof blob - 1, "world", 5);
    if (!m || ((char *)m)[0] != 'w' || ((char *)m)[-1] != '-')
        return 16;
    stf = stat;
    fd = open(path, O_CREAT | O_TRUNC | O_RDWR, 0600);
    if (fd < 0)
        return 17;
    if (write(fd, "z", 1) != 1)
        return 18;
    close(fd);
    if (stf(path, &st) != 0 || st.st_size != 1)
        return 19;
    unlink(path);
    return 0;
}
