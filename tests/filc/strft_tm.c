#include <time.h>

int main(void)
{
    char buf[8];

    (void)strftime(buf, 8, "%Y", 0);
    return 0;
}
