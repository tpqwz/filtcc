#include <string.h>

int main(void)
{
    char *p = index("hello", 'e');
    *p = 'E';
    return 0;
}
