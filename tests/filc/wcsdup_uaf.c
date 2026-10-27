#include <wchar.h>
#include <stdlib.h>

int main(void)
{
    wchar_t *p = wcsdup(L"ab");
    free(p);
    return p[0];
}
