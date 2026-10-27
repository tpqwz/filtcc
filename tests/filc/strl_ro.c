#include <string.h>

int main(void)
{
    (void)strlcpy((char *)"0123456789abcdef", "ab", 4);
    return 0;
}
