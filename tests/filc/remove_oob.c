#include <stdio.h>

int main(void)
{
    char path[3] = {'a', 'b', 'c'};
    (void)remove(path);
    return 0;
}
