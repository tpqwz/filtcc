#include <stdlib.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    char buf[4096];
    (void)realpath(p, buf);
    return 0;
}
