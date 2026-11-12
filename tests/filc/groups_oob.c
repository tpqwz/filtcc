#include <unistd.h>
int main(void)
{
    gid_t b[2];
    (void)getgroups(64, b);
    return 0;
}
