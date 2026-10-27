#include <wctype.h>

int main(void)
{
    wctype_t (*fn)(const char *);
    char buf[4];
    buf[0] = 'a';
    buf[1] = 'l';
    buf[2] = 'p';
    buf[3] = 'h';
    fn = wctype;
    (void)fn(buf);
    return 0;
}
