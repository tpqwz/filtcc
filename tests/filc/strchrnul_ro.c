#include <string.h>

int main(void)
{
    char *p = strchrnul("hello", 'e');
    *p = 'E';
    return 0;
}
