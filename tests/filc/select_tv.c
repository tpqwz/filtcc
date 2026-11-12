#include <sys/select.h>
#include <sys/time.h>
int main(void)
{
    char b[4];
    (void)select(0, 0, 0, 0, (struct timeval *)b);
    return 0;
}
