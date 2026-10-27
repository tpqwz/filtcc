#include <string.h>

int main(void)
{
    char a[8];
    char b[2];
    a[0] = 'a';
    b[0] = 'a';
    b[1] = 'b';
    (void)bcmp(a, b, 4);
    return 0;
}
