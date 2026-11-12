#include <stdlib.h>
int main(void)
{
    (void)mkdtemp("/tmp/filc_rd_XXXXXX");
    return 0;
}
