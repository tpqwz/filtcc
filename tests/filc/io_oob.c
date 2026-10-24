#include <unistd.h>

int main(void)
{
    char buf[4];
    (void)read(-1, buf, 8);
    return 0;
}
