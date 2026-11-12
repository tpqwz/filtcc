#include <fcntl.h>
#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)fchownat(AT_FDCWD, p, 0, 0, 0);
    return 0;
}
