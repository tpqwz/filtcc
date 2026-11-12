#include <fcntl.h>
#include <sys/stat.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    struct stat st;
    (void)fstatat(AT_FDCWD, p, &st, 0);
    return 0;
}
