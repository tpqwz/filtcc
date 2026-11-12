#include <signal.h>
int main(void)
{
    stack_t old;
    int n;
    int (*fn)(const stack_t *, stack_t *);
    n = sigaltstack(0, &old);
    if (n != 0)
        return 2;
    n = sigaltstack(0, 0);
    if (n != 0)
        return 3;
    fn = sigaltstack;
    n = fn(0, &old);
    if (n != 0)
        return 4;
    return 0;
}
