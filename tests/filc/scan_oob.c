#include <string.h>

int main(void)
{
    char buf[4] = {'a', 'a', 'a', 'a'};
    /* No terminator. strspn must not walk off the object. */
    (void)strspn(buf, "a");
    return 0;
}
