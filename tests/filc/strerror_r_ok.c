#include <errno.h>
#include <string.h>

int main(void)
{
    char buf[64];
    char tiny[4];
    char one;
    int (*f)(int, char *, size_t);
    int r;

    r = strerror_r(0, buf, sizeof buf);
    if (r != 0 || strcmp(buf, "Success") != 0)
        return 2;
    r = strerror_r(ENOENT, buf, sizeof buf);
    if (r != 0 || strcmp(buf, "No such file or directory") != 0)
        return 3;
    tiny[0] = tiny[1] = tiny[2] = tiny[3] = 'Q';
    r = strerror_r(ENOENT, tiny, 4);
    if (r != ERANGE || tiny[0] != 'N' || tiny[1] != 'o'
        || tiny[2] != ' ' || tiny[3] != 0)
        return 4;
    one = 'Q';
    r = strerror_r(ENOENT, &one, 0);
    if (r != ERANGE || one != 'Q')
        return 5;
    r = strerror_r(ENOENT, 0, 0);
    if (r != ERANGE)
        return 6;
    r = strerror_r(0, (char *)"x", 0);
    if (r != ERANGE)
        return 7;
    r = strerror_r(99999, buf, sizeof buf);
    if (r != EINVAL || strcmp(buf, "Unknown error 99999") != 0)
        return 8;
    f = strerror_r;
    r = f(0, buf, sizeof buf);
    if (r != 0 || strcmp(buf, "Success") != 0)
        return 9;
    r = f(ENOENT, 0, 0);
    if (r != ERANGE)
        return 10;
    return 0;
}
