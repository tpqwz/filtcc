#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void filc_collect(void);
unsigned long filc_freed_count(void);

static uintptr_t hidden;

__attribute__((noinline)) static int arm(FILE *fp)
{
    char *buf = malloc(4096);
    if (!buf)
        return 2;
    hidden = (uintptr_t)buf;
    buf[0] = 1;
    if (setvbuf(fp, buf, _IOFBF, 4096) != 0)
        return 3;
    return 0;
}

__attribute__((noinline)) static void scrub(void)
{
    volatile char buf[8192];
    int i;
    for (i = 0; i < 8192; i++)
        buf[i] = 0;
}

int main(void)
{
    FILE *fp;
    unsigned long before;
    int rc;
    fp = tmpfile();
    if (!fp)
        return 1;
    scrub();
    filc_collect();
    before = filc_freed_count();
    rc = arm(fp);
    if (rc)
        return rc;
    scrub();
    filc_collect();
    /* The stream still owns the heap buffer. */
    if (filc_freed_count() != before)
        return 4;
    if (fputc('Z', fp) != 'Z')
        return 5;
    if (fflush(fp) != 0)
        return 6;
    rewind(fp);
    if (fgetc(fp) != 'Z')
        return 7;
    if (fclose(fp) != 0)
        return 8;
    hidden = 0;
    scrub();
    before = filc_freed_count();
    filc_collect();
    /* fclose drops the pin, so the buffer can be reclaimed. */
    if (filc_freed_count() == before)
        return 9;
    return 0;
}
