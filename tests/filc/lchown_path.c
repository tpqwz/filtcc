#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)lchown(p, 0, 0);
    return 0;
}
