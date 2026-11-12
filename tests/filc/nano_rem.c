#include <time.h>
int main(void)
{
    struct timespec req;
    char b[4];
    req.tv_sec = 0;
    req.tv_nsec = 1000;
    (void)nanosleep(&req, (struct timespec *)b);
    return 0;
}
