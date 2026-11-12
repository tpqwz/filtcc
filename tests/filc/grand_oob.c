#include <sys/random.h>
int main(void)
{
    char b[4];
    (void)getrandom(b, 64, 0);
    return 0;
}
