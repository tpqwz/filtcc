#include <stdlib.h>
#include <locale.h>

int main(void)
{
    char tiny[1];
    if (!setlocale(LC_ALL, "C.UTF-8"))
        return 1;
    tiny[0] = 0;
    (void)wctomb(tiny, L'a');
    return 0;
}
