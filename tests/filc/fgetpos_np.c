#include <stdio.h>

int main(void)
{
    FILE *fp;
    fp = tmpfile();
    if (!fp)
        return 1;
    (void)fgetpos(fp, 0);
    return 0;
}
