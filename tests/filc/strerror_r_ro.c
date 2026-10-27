#include <string.h>

int main(void)
{
    strerror_r(0, (char *)"xxxxxxxx", 4);
    return 0;
}
