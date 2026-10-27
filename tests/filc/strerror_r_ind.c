#include <string.h>

int main(void)
{
    int (*f)(int, char *, size_t) = strerror_r;
    char buf[4];
    f(0, buf, 8);
    return 0;
}
