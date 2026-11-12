#include <fcntl.h>
#include <sys/stat.h>
int main(void)
{
    char b[8];
    (void)fstatat(AT_FDCWD, "/", (struct stat *)b, 0);
    return 0;
}
