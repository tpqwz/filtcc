#include <fcntl.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)renameat(AT_FDCWD, p, AT_FDCWD, "x");
    return 0;
}
