#include <string.h>

int main(void)
{
    char a[4];
    char b[4];
    (void)bcmp(a, b, (size_t)-1);
    return 0;
}
