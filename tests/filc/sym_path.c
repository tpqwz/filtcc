#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)symlink("x", p);
    return 0;
}
