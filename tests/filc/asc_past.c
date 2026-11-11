#include <string.h>
#include <time.h>

int main(void)
{
    time_t t = 0;
    char *s = asctime(gmtime(&t));
    volatile char c;

    c = s[strlen(s) + 1];
    (void)c;
    return 0;
}
