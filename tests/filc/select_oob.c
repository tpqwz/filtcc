#include <sys/select.h>
#include <sys/time.h>
int main(void)
{
    char b[8];
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    (void)select(1, (fd_set *)b, 0, 0, &tv);
    return 0;
}
