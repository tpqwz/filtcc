#include <fcntl.h>

int main(void)
{
    char path[4] = {'/', 't', 'm', 'p'};
    (void)open(path, O_RDONLY);
    return 0;
}
