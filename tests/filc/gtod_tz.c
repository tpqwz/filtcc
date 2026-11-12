#include <sys/time.h>
int main(void)
{
    struct timeval tv;
    char z[1];
    (void)gettimeofday(&tv, (struct timezone *)z);
    return 0;
}
