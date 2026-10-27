#include <wctype.h>

int main(void)
{
    char buf[4];
    buf[0] = 'a';
    buf[1] = 'l';
    buf[2] = 'p';
    buf[3] = 'h';
    (void)wctype(buf);
    return 0;
}
