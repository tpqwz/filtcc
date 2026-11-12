#include <unistd.h>
int main(void)
{
    char p[4] = {'/', 't', 'm', 'p'};
    (void)link("/etc/passwd", p);
    return 0;
}
