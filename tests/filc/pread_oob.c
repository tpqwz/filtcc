#include <unistd.h>

int main(void)
{
    char buf[4];
    (void)pread(-1, buf, 8, 0);
    return 0;
}
