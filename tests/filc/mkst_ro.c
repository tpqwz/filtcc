#include <stdlib.h>
int main(void)
{
    (void)mkstemp("/tmp/filc_ro_XXXXXX");
    return 0;
}
