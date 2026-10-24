#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void filc_collect(void);
unsigned long filc_freed_count(void);

static volatile int phase;

__attribute__((noinline)) static void scrub(void)
{
    volatile char buf[8192];
    int i;
    for (i = 0; i < 8192; i++)
        buf[i] = 0;
}

/* Spin inside the scrubbed frame. The stop handler's stack is below this
 * buffer, so the collector's scan sees zeros instead of churn's pointers. */
__attribute__((noinline)) static void hold(void)
{
    volatile char buf[8192];
    int i;
    for (i = 0; i < 8192; i++)
        buf[i] = 0;
    phase = 1;
    while (phase == 1)
        ;
    if (buf[0] != 0)
        phase = 3;
}

__attribute__((noinline)) static void churn(void)
{
    int i;
    /* Stay under the 256KB automatic threshold so the collector is main. */
    for (i = 0; i < 20; i++) {
        char *p = malloc(4096);
        if (!p)
            _exit(2);
        p[0] = 1;
        p[4095] = 2;
        free(p);
    }
}

static void *worker(void *arg)
{
    (void)arg;
    churn();
    hold();
    return 0;
}

int main(void)
{
    pthread_t th;
    char *keep;
    unsigned long before;
    before = filc_freed_count();
    if (pthread_create(&th, 0, worker, 0) != 0)
        return 2;
    while (phase == 0)
        ;
    keep = malloc(32);
    if (!keep)
        return 2;
    keep[0] = 7;
    keep[31] = 9;
    scrub();
    filc_collect();
    if (keep[0] != 7 || keep[31] != 9)
        return 4;
    if (filc_freed_count() == before)
        return 3;
    phase = 2;
    if (pthread_join(th, 0) != 0)
        return 5;
    return 0;
}
