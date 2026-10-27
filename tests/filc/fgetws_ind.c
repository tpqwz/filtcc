#include <stdio.h>
#include <wchar.h>

int main(void)
{
    wchar_t *(*fn)(wchar_t *, int, FILE *);
    wchar_t buf[2];
    fn = fgetws;
    fn(buf, 8, stdin);
    return 0;
}
