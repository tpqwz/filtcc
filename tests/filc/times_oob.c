#include <sys/times.h>
int main(void)
{
    char b[4];
    (void)times((struct tms *)b);
    return 0;
}
