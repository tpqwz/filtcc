#include <wchar.h>

int main(void)
{
    wchar_t w[8];
    swscanf(L"hi", L"%ls", w);
    return 0;
}
