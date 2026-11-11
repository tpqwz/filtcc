#include <time.h>

int main(void)
{
    (void)mktime((struct tm *)"0123456789");
    return 0;
}
