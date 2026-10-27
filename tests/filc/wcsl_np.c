#include <wchar.h>

int main(void)
{
    (void)wcslcpy(0, L"a", 4);
    return 0;
}
