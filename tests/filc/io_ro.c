#include <unistd.h>

int main(void)
{
    (void)read(-1, "hi", 2);
    return 0;
}
