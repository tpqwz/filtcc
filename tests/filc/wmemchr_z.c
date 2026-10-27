#include <wchar.h>

int main(void)
{
    (void)wmemchr(0, L'a', 0);
    return 0;
}
