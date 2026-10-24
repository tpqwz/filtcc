#include <unistd.h>

int main(void)
{
    char buf[4] = {'a', 'b', 'c', 'd'};
    (void)write(-1, buf, 8);
    return 0;
}
