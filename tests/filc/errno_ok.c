#include <errno.h>

int main(void)
{
    errno = 0;
    if (errno != 0)
        return 1;
    errno = EIO;
    if (errno != EIO)
        return 2;
    return 0;
}
