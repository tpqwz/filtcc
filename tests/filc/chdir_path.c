#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)chdir(p);
    return 0;
}
