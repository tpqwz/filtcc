#include <time.h>

int main(void)
{
    char tiny;

    (void)asctime((struct tm *)&tiny);
    return 0;
}
