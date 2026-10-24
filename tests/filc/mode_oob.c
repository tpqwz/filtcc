#include <stdio.h>

int main(void)
{
    char mode[1] = {'r'};
    (void)fopen("/dev/null", mode);
    return 0;
}
