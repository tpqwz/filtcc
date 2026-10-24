#include <sys/stat.h>

int main(void)
{
    char p[3] = {'a', 'b', 'c'};
    struct stat st;
    stat(p, &st);
    return 0;
}
