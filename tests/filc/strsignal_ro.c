#include <signal.h>
#include <string.h>

int main(void)
{
    char *s = strsignal(SIGSEGV);
    s[0] = 'X';
    return 0;
}
