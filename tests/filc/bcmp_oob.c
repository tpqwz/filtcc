#include <string.h>

int main(void)
{
    char a[2];
    char b[8];
    a[0] = 'a';
    a[1] = 'b';
    (void)bcmp(a, b, 4);
    return 0;
}
