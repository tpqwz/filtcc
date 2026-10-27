#define _GNU_SOURCE
#include <dlfcn.h>
#include <elf.h>
#include <string.h>

static int call_rand(void *p)
{
    int (*f)(void);

    f = p;
    if (!f)
        return 0;
    (void)f();
    return 1;
}

int main(void)
{
    void *h, *p, *extra;
    Dl_info info;
    const Elf64_Sym *sym;
    unsigned long bits;
    int x;
    float (*af)(float);
    void *(*mo)(Lmid_t, const char *, int);
    void *(*vs)(void *, const char *, const char *);
    int (*da)(const void *, Dl_info *);
    int (*da1)(const void *, Dl_info *, void **, int);
    const char *err, *s;

    p = dlvsym(RTLD_DEFAULT, "rand", "GLIBC_2.2.5");
    if (!call_rand(p))
        return 1;
    if (dlvsym(RTLD_DEFAULT, "rand", "NO_SUCH_VERSION"))
        return 2;
    err = dlerror();
    if (!err || !err[0])
        return 3;
    while (*err)
        err++;

    h = dlmopen(LM_ID_BASE, "libdl.so.2", RTLD_LAZY);
    if (!h || !dlsym(h, "dlsym"))
        return 4;
    if (dlclose(h) != 0)
        return 5;

    h = dlmopen(LM_ID_NEWLM, "libm.so.6", RTLD_NOW);
    if (!h)
        return 6;
    af = dlvsym(h, "atanhf", "GLIBC_2.2.5");
    if (!af || af(0.0f) != 0.0f)
        return 7;
    if (dladdr(af, &info) == 0 || !info.dli_sname
        || strcmp(info.dli_sname, "atanhf") != 0)
        return 8;
    if (dlclose(h) != 0)
        return 9;

    p = dlsym(RTLD_DEFAULT, "optind");
    if (!p || dladdr(p, &info) == 0)
        return 10;
    if (!info.dli_fname || !info.dli_fname[0])
        return 11;
    s = info.dli_fname;
    while (*s)
        s++;
    if (!info.dli_sname || strcmp(info.dli_sname, "optind") != 0)
        return 12;
    if (info.dli_saddr != p || *(int *)info.dli_saddr != *(int *)p)
        return 13;
    if (!info.dli_fbase)
        return 14;
    {
        unsigned char *b = info.dli_fbase;
        if (b[0] != 0x7f || b[1] != 'E' || b[2] != 'L' || b[3] != 'F')
            return 15;
    }

    bits = (unsigned long)p;
    if (dladdr((void *)bits, &info) == 0 || !info.dli_sname)
        return 16;
    if (dladdr(&x, &info) != 0)
        return 17;

    extra = 0;
    if (dladdr1(p, &info, &extra, RTLD_DL_SYMENT) == 0 || !extra)
        return 18;
    sym = extra;
    if (sym->st_size != 4)
        return 19;
    extra = 0;
    if (dladdr1(p, &info, &extra, RTLD_DL_LINKMAP) == 0 || !extra)
        return 20;

    mo = dlmopen;
    h = mo(LM_ID_BASE, NULL, RTLD_LAZY);
    if (!h || dlclose(h) != 0)
        return 21;
    vs = dlvsym;
    if (!call_rand(vs(RTLD_DEFAULT, "rand", "GLIBC_2.2.5")))
        return 22;
    da = dladdr;
    if (da(p, &info) == 0 || !info.dli_fname)
        return 23;
    da1 = dladdr1;
    extra = 0;
    if (da1(p, &info, &extra, RTLD_DL_SYMENT) == 0 || !extra)
        return 24;
    sym = extra;
    if (sym->st_size != 4)
        return 25;
    return 0;
}
