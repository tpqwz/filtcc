#define _GNU_SOURCE
#include <dlfcn.h>
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
    void *h, *h2, *mainh, *p, *slot;
    void *(*sym)(void *, const char *);
    size_t (*sl)(const char *);
    int (*rf)(void);
    int *oi;
    const char *err;
    unsigned long bits;
    struct { void *handle; } box;
    char b;

    h = dlopen("libc.so.6", RTLD_LAZY);
    if (!h)
        return 1;
    slot = h;
    rf = dlsym(slot, "rand");
    if (!call_rand(rf))
        return 2;
    box.handle = h;
    rf = dlsym(box.handle, "rand");
    if (!call_rand(rf))
        return 3;

    sl = dlsym(RTLD_NEXT, "strlen");
    if (!sl || sl("hi") != 2)
        return 4;

    oi = dlsym(RTLD_DEFAULT, "optind");
    if (!oi)
        return 5;
    (void)*oi;

    p = dlsym(h, "_IO_2_1_stdout_");
    if (!p)
        return 6;
    b = *(char *)p;
    (void)b;

    /* The compiler stores a resolved address as an integer. */
    bits = (unsigned long)dlsym(h, "rand");
    if (!bits)
        return 7;

    p = dlsym(h, "filc_no_such_symbol");
    if (p)
        return 8;
    err = dlerror();
    if (!err || !err[0])
        return 9;
    while (*err)
        err++;
    if (dlerror() != NULL)
        return 10;

    p = dlopen("filc-no-such-lib.so", RTLD_LAZY);
    if (p)
        return 11;
    err = dlerror();
    if (!err || !err[0])
        return 12;
    while (*err)
        err++;

    sym = dlsym;
    rf = sym(h, "rand");
    if (!call_rand(rf))
        return 13;

    h2 = dlopen("libc.so.6", RTLD_LAZY);
    if (h2 != h)
        return 14;
    if (dlclose(h2) != 0)
        return 15;
    rf = dlsym(h, "rand");
    if (!call_rand(rf))
        return 16;

    mainh = dlopen(NULL, RTLD_LAZY);
    if (!mainh)
        return 17;
    if (dlclose(mainh) != 0)
        return 18;

    if (dlclose(h) != 0)
        return 19;
    /* libc stays mapped, so the handle capability stays. */
    rf = dlsym(h, "rand");
    if (!call_rand(rf))
        return 20;

    /* Five-byte object in a non-writable load segment. */
    p = dlsym(RTLD_DEFAULT, "_libc_intl_domainname");
    if (!p)
        return 21;
    {
        char *s = p;
        int i;
        for (i = 0; i < 5; i++)
            b = s[i];
    }
    (void)b;
    return 0;
}
