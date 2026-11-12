#include <sys/statvfs.h>
int main(void)
{
    char b[8];
    (void)statvfs("/", (struct statvfs *)b);
    return 0;
}
