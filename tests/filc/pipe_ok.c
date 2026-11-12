#include <unistd.h>
int main(void)
{
    int fds[2];
    int n;
    int (*fn)(int *);
    n = pipe(fds);
    if (n != 0 || fds[0] < 0 || fds[1] < 0)
        return 2;
    close(fds[0]);
    close(fds[1]);
    fn = pipe;
    n = fn(fds);
    if (n != 0 || fds[0] < 0 || fds[1] < 0)
        return 3;
    close(fds[0]);
    close(fds[1]);
    return 0;
}
