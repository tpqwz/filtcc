#include <unistd.h>

int main(void)
{
    (void)read(-1, 0, 0);
    return 0;
}
