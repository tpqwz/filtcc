#include <time.h>
int main(void)
{
    char b[4];
    (void)nanosleep((struct timespec *)b, 0);
    return 0;
}
