#include <errno.h>
#include <string.h>

int main(void)
{
    char *s = strerror(ENOENT);
    s[0] = 'X';
    return 0;
}
