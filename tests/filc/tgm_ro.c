#include <time.h>

int main(void)
{
    (void)timegm((struct tm *)"0123456789");
    return 0;
}
