#include <sys/stat.h>

int main(void)
{
    char b[4];
    stat("/etc/passwd", (struct stat *)b);
    return 0;
}
