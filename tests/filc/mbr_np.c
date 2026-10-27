#include <wchar.h>

int main(void)
{
    mbstate_t st = {0};
    (void)mbrlen(0, 1, &st);
    return 0;
}
