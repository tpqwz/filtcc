#include <time.h>

int main(void)
{
    time_t t = 0;

    *asctime(gmtime(&t)) = 'Z';
    return 0;
}
