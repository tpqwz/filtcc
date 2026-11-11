#include <sys/resource.h>
int main(void)
{
    char b[16];
    (void)getrusage(RUSAGE_SELF, (struct rusage *)b);
    return 0;
}
