#include <sys/sysinfo.h>
int main(void)
{
    char b[16];
    (void)sysinfo((struct sysinfo *)b);
    return 0;
}
