#include <string.h>

int main(void)
{
    char buf[4];
    strerror_r(0, buf, 8);
    return 0;
}
