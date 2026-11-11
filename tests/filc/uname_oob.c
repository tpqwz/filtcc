#include <sys/utsname.h>
int main(void)
{
    char b[64];
    (void)uname((struct utsname *)b);
    return 0;
}
