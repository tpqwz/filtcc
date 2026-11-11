#include <unistd.h>
int main(void)
{
    char b[64];
    (void)gethostname(b, 4096);
    return 0;
}
