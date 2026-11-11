#include <time.h>

int main(void)
{
    char buf[26];

    (void)ctime_r(0, buf);
    return 0;
}
