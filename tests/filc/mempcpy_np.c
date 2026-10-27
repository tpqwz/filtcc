#include <string.h>

int main(void)
{
    (void)mempcpy(0, "a", 1);
    return 0;
}
