#include <string.h>

int main(void)
{
    char *s = strerror(0);
    volatile char c;
    size_t n = 0;
    while (s[n])
        n++;
    c = s[n + 1];
    (void)c;
    return 0;
}
