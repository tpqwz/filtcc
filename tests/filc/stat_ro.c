#include <sys/stat.h>

int main(void)
{
    const struct stat st;
    stat("/etc/passwd", (struct stat *)&st);
    return 0;
}
