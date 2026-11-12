#include <sys/stat.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)chmod(p, 0700);
    return 0;
}
