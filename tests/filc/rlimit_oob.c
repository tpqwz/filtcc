#include <sys/resource.h>
int main(void)
{
    char b[4];
    (void)getrlimit(RLIMIT_NOFILE, (struct rlimit *)b);
    return 0;
}
