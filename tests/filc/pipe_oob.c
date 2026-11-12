#include <unistd.h>
int main(void)
{
    char b[2];
    (void)pipe((int *)b);
    return 0;
}
