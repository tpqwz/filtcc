#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char path[64];
    char *p = "abcd";
    size_t n = 4;
    FILE *fp;

    snprintf(path, sizeof path, "/tmp/filc-glr-%d", (int)getpid());
    fp = fopen(path, "w+");
    if (!fp)
        return 1;
    fputs("a\n", fp);
    rewind(fp);
    (void)getline(&p, &n, fp);
    fclose(fp);
    remove(path);
    return 0;
}
