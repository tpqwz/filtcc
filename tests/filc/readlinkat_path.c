#include <fcntl.h>
#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    char buf[8];
    (void)readlinkat(AT_FDCWD, p, buf, sizeof buf);
    return 0;
}
