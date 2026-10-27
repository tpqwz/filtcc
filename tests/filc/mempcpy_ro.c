#include <string.h>

int main(void)
{
    (void)mempcpy((char *)"0123456789abcdef", "ab", 2);
    return 0;
}
