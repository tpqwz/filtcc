#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void)
{
    struct stat st;
    int n;
    int (*fn)(int, const char *, struct stat *, int);
    int (*acc)(int, const char *, int, int);
    n = fstatat(AT_FDCWD, "/dev/null", &st, 0);
    if (n != 0 || st.st_mode == 0)
        return 2;
    n = faccessat(AT_FDCWD, "/dev/null", R_OK, 0);
    if (n != 0)
        return 3;
    fn = fstatat;
    n = fn(AT_FDCWD, "/dev/null", &st, 0);
    if (n != 0 || st.st_mode == 0)
        return 4;
    acc = faccessat;
    n = acc(AT_FDCWD, "/dev/null", R_OK, 0);
    if (n != 0)
        return 5;
    return 0;
}
