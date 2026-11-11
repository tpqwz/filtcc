#include <time.h>

int main(void)
{
    char tiny;
    char *(*fa)(const struct tm *);

    fa = asctime;
    (void)fa((struct tm *)&tiny);
    return 0;
}
