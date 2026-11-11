#include <unistd.h>
int main(void)
{
    char b[64];
    (void)getdomainname(b, 4096);
    return 0;
}
