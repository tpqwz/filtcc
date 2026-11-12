#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
int main(void)
{
    int fd;
    char b[4];
    fd = open("/dev/null", O_RDONLY);
    if (fd < 0)
        return 2;
    (void)futimens(fd, (struct timespec *)b);
    close(fd);
    return 0;
}
