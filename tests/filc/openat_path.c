#include <fcntl.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)openat(AT_FDCWD, p, O_RDONLY);
    return 0;
}
