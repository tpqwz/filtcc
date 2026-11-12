#include <sys/time.h>
int main(void)
{
    char b[4];
    (void)gettimeofday((struct timeval *)b, 0);
    return 0;
}
