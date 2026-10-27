#include <wctype.h>

int main(void)
{
    wctrans_t (*fn)(const char *);
    char buf[7];
    buf[0] = 't';
    buf[1] = 'o';
    buf[2] = 'l';
    buf[3] = 'o';
    buf[4] = 'w';
    buf[5] = 'e';
    buf[6] = 'r';
    fn = wctrans;
    (void)fn(buf);
    return 0;
}
