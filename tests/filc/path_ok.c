#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char path[] = "/tmp/filc-path-ok.dat";
    char buf[32];
    int fd, saved;
    FILE *fp;
    size_t n;

    if (fopen("/tmp/filc-no-such-dir/x", "r") != 0)
        return 1;
    fd = open("/dev/null", O_RDONLY);
    if (fd < 0)
        return 2;
    close(fd);

    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
        return 3;
    fp = fdopen(fd, "w");
    if (!fp)
        return 4;
    if (fputs("ab", fp) < 0)
        return 5;
    if (fclose(fp) != 0)
        return 6;

    fp = fopen(path, "a");
    if (!fp)
        return 7;
    if (fputs("cd", fp) < 0)
        return 8;
    if (fclose(fp) != 0)
        return 9;

    fflush(stdout);
    saved = dup(1);
    if (saved < 0)
        return 10;
    fd = open(path, O_WRONLY | O_APPEND);
    if (fd < 0)
        return 11;
    if (dup2(fd, 1) < 0)
        return 12;
    if (puts("z") < 0)
        return 13;
    fflush(stdout);
    dup2(saved, 1);
    close(saved);
    close(fd);

    fp = fopen(path, "r");
    if (!fp)
        return 14;
    memset(buf, 0, sizeof buf);
    n = fread(buf, 1, sizeof buf - 1, fp);
    fclose(fp);
    if (n != 6 || memcmp(buf, "abcdz\n", 6) != 0)
        return 15;

    fp = freopen(path, "r", stdin);
    if (!fp)
        return 16;
    memset(buf, 0, sizeof buf);
    if (!fgets(buf, 16, stdin) || strcmp(buf, "abcdz\n") != 0)
        return 17;
    fp = fopen(path, "r");
    if (!fp)
        return 18;
    /* A null path reopens the same file. */
    fp = freopen(0, "r", fp);
    if (!fp)
        return 19;
    memset(buf, 0, sizeof buf);
    if (!fgets(buf, 16, fp) || strcmp(buf, "abcdz\n") != 0)
        return 20;
    fclose(fp);
    unlink(path);
    return 0;
}
