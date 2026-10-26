#include <unistd.h>

int main(void)
{
    char b[8];
    (void)getcwd(b, 4096);
    return 0;
}
