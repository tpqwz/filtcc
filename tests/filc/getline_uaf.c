#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char path[64];
    char *p, *old;
    size_t n;
    FILE *fp;

    snprintf(path, sizeof path, "/tmp/filc-glu-%d", (int)getpid());
    fp = fopen(path, "w+");
    if (!fp)
        return 1;
    fputs("hello\n", fp);
    rewind(fp);
    p = malloc(4);
    if (!p)
        return 2;
    n = 4;
    old = p;
    if (getline(&p, &n, fp) < 0 || p == old)
        return 3;
    old[0] = 1;
    free(p);
    fclose(fp);
    remove(path);
    return 0;
}
