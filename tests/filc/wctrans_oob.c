#include <wctype.h>

int main(void)
{
    char buf[7];
    buf[0] = 't';
    buf[1] = 'o';
    buf[2] = 'l';
    buf[3] = 'o';
    buf[4] = 'w';
    buf[5] = 'e';
    buf[6] = 'r';
    (void)wctrans(buf);
    return 0;
}
