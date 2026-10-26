#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char src[64], sop[64], cmd[160];
    FILE *f;
    void *h;
    int (*fn)(void);
    int id = (int)getpid();

    snprintf(src, sizeof src, "/tmp/filc-dl-%d.c", id);
    snprintf(sop, sizeof sop, "/tmp/filc-dl-%d.so", id);
    f = fopen(src, "w");
    if (!f)
        return 1;
    if (fputs("int filc_dl_answer(void){return 42;}\n", f) < 0) {
        fclose(f);
        return 1;
    }
    if (fclose(f) != 0)
        return 1;
    snprintf(cmd, sizeof cmd, "./tcc -B. -shared -o %s %s", sop, src);
    if (system(cmd) != 0)
        return 1;
    h = dlopen(sop, RTLD_NOW);
    if (!h)
        return 1;
    fn = dlsym(h, "filc_dl_answer");
    if (!fn || fn() != 42)
        return 1;
    if (dlclose(h) != 0)
        return 1;
    dlsym(h, "filc_dl_answer");
    return 0;
}
