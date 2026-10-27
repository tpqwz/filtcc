#include <inttypes.h>
#include <wchar.h>

int main(void)
{
    wchar_t src[2];
    wchar_t *end;
    src[0] = L'1';
    src[1] = L'2';
    (void)wcstoimax(src, &end, 10);
    return 0;
}
