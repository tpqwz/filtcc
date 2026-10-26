#include <unistd.h>

int main(void)
{
    char *s = "hi";
    (void)getcwd(s, 2);
    return 0;
}
