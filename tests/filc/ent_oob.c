#include <unistd.h>
int main(void)
{
    char b[4];
    (void)getentropy(b, 64);
    return 0;
}
