#include <stdio.h>

int main(void)
{
    char path[3] = {'a', 'b', 'c'};
    (void)fopen(path, "r");
    return 0;
}
