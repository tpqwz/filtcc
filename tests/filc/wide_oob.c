#include <stdlib.h>
#include <wchar.h>

int main(void)
{
    wchar_t *d = malloc(sizeof(wchar_t));
    if (!d)
        return 2;
    wcscpy(d, L"hi");
    return 0;
}
