#include <fcntl.h>
#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)symlinkat("x", AT_FDCWD, p);
    return 0;
}
