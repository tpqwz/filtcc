#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(void)
{
    char path[] = "/tmp/filc_md_XXXXXX";
    char *r;
    char *(*fn)(char *);
    r = mkdtemp(path);
    if (!r || strcmp(r, path) != 0)
        return 2;
    if (rmdir(path) != 0)
        return 3;
    memcpy(path, "/tmp/filc_md_XXXXXX", 20);
    fn = mkdtemp;
    r = fn(path);
    if (!r || strcmp(r, path) != 0)
        return 2;
    if (rmdir(path) != 0)
        return 3;
    return 0;
}
