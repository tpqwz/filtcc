#include <inttypes.h>

static char *const roend = 0;

int main(void)
{
    return (int)strtoimax("12", (char **)&roend, 10);
}
