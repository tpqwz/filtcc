#include <string.h>

int main(void)
{
    explicit_bzero("hi", 2);
    return 0;
}
