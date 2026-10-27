#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int main(void)
{
    wchar_t *p = malloc(sizeof(wchar_t));
    char buf[8];
    if (!p)
        return 2;
    p[0] = L'Z';
    sprintf(buf, "%ls", p);
    return 0;
}
