#include <wchar.h>
#include <locale.h>

int main(void)
{
    char tiny[1];
    mbstate_t st = {0};
    if (!setlocale(LC_ALL, "C.UTF-8"))
        return 1;
    tiny[0] = 0;
    (void)wcrtomb(tiny, L'a', &st);
    return 0;
}
