#include <wchar.h>

int main(void)
{
    (void)wmemchr(0, L'a', 1);
    return 0;
}
