#include <string.h>

int main(void)
{
    strerror_r(0, 0, 8);
    return 0;
}
