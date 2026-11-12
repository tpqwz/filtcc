#include <stdlib.h>
int main(void)
{
    char t[4] = {'a', 'b', 'c', 'd'};
    (void)mkdtemp(t);
    return 0;
}
