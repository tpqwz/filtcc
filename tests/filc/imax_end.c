#include <inttypes.h>

int main(void)
{
    char tiny;
    (void)strtoimax("12", (char **)&tiny, 10);
    return 0;
}
