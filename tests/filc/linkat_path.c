#include <fcntl.h>
#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)linkat(AT_FDCWD, "/etc/passwd", AT_FDCWD, p, 0);
    return 0;
}
