#include <string.h>

int main(void)
{
    char buf[3] = {'a', 'b', 'c'};
    char *save = 0;
    /* No terminator and no delimiter, so the scan steps off the object. */
    (void)strtok_r(buf, ",", &save);
    return 0;
}
