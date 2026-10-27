#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char path[64];
    char buf[4];
    FILE *fp;
    int i, c;

    snprintf(path, sizeof path, "/tmp/filc-fco-%d", (int)getpid());
    fp = fopen(path, "w+");
    if (!fp)
        return 1;
    fputs("hello", fp);
    rewind(fp);
    i = 0;
    while ((c = fgetc(fp)) != EOF && i < 8)
        buf[i++] = (char)c;
    fclose(fp);
    remove(path);
    return 0;
}
