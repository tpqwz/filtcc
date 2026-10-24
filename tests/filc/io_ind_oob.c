#include <unistd.h>

int main(void)
{
    ssize_t (*rd)(int, void *, size_t) = read;
    char buf[4];
    (void)rd(-1, buf, 8);
    return 0;
}
