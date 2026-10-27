#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char path[64];
    char buf[4];
    char *p;
    size_t n;
    FILE *fp;

    snprintf(path, sizeof path, "/tmp/filc-gl-%d", (int)getpid());
    fp = fopen(path, "w+");
    if (!fp)
        return 1;
    fputs("hello\n", fp);
    rewind(fp);
    p = buf;
    n = sizeof buf;
    (void)getline(&p, &n, fp);
    fclose(fp);
    remove(path);
    return 0;
}
