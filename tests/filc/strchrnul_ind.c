#include <string.h>

int main(void)
{
    char *(*fn)(const char *, int);
    char src[1];
    src[0] = 'A';
    fn = strchrnul;
    (void)fn(src, 'Z');
    return 0;
}
