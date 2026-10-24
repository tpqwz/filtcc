#include <stdlib.h>
__attribute__((noinline)) static void use(int n)
{
    char *p = alloca(n);
    int i;
    for (i = 0; i < n; i++)
        p[i] = (char)i;
    for (i = 0; i < n; i++)
        if (p[i] != (char)i)
            _exit(2);
}
int main(void)
{
    int x = 42;
    use(32);
    if (x != 42)
        return 1;
    return 0;
}
