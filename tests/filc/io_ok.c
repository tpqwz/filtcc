#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char buf[32];
    char small[8];
    char path[] = "/tmp/filc-io-ok.dat";
    int fd, zfd;
    FILE *fp;
    char *p;
    ssize_t n;
    unsigned long slot;
    int x = 7;
    int *px = &x;
    int *q;
    unsigned char *b;
    unsigned char *bits;
    int i;

    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
        return 1;
    if (write(fd, "hello\n", 6) != 6)
        return 2;
    /* A read-only source is a legal write. */
    if (write(fd, "hi", 2) != 2)
        return 3;
    if (write(fd, buf, 0) != 0)
        return 4;
    if (lseek(fd, 0, SEEK_SET) != 0)
        return 5;
    memset(buf, 0, sizeof buf);
    n = read(fd, buf, 6);
    if (n != 6 || memcmp(buf, "hello\n", 6) != 0)
        return 6;
    if (read(fd, buf, 0) != 0)
        return 7;
    if (pread(fd, small, 2, 1) != 2 || small[0] != 'e' || small[1] != 'l')
        return 8;
    if (pwrite(fd, "XY", 2, 0) != 2)
        return 9;
    if (pread(fd, small, 2, 0) != 2 || small[0] != 'X' || small[1] != 'Y')
        return 10;
    close(fd);

    fp = fopen(path, "w+");
    if (!fp)
        return 11;
    if (fwrite("ab\ncd", 1, 5, fp) != 5)
        return 12;
    if (fwrite(buf, 1, 0, fp) != 0)
        return 13;
    if (fwrite(buf, 0, 4, fp) != 0)
        return 14;
    rewind(fp);
    memset(buf, 0, sizeof buf);
    p = fgets(buf, 16, fp);
    if (!p || p != buf || strcmp(buf, "ab\n") != 0)
        return 15;
    /* Interior return keeps the buffer capability. */
    p = fgets(buf + 2, 8, fp);
    if (!p || p != buf + 2 || p[0] != 'c' || p[1] != 'd' || p[2] != 0)
        return 16;
    if (fread(small, 1, 1, fp) != 0)
        return 17;
    if (fread(small, 0, 4, fp) != 0)
        return 18;
    if (fgets(buf, 8, fp) != 0)
        return 19;
    fclose(fp);

    /* A kernel read overwrites integer bits and leaves the capability. */
    memcpy(&slot, &px, sizeof px);
    zfd = open("/dev/zero", O_RDONLY);
    if (zfd < 0)
        return 20;
    if (read(zfd, &slot, sizeof slot) != (ssize_t)sizeof slot)
        return 21;
    close(zfd);
    b = (unsigned char *)&slot;
    bits = (unsigned char *)&px;
    for (i = 0; i < (int)sizeof slot; i++)
        b[i] = bits[i];
    q = *(int **)&slot;
    if (q != &x || *q != 7)
        return 22;

    unlink(path);
    return 0;
}
