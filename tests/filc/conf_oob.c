#include <unistd.h>
int main(void)
{
    char b[64];
    (void)confstr(_CS_PATH, b, 4096);
    return 0;
}
