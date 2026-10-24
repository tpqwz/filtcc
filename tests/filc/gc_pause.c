#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/* The worker blocks in sigsuspend(), outside the filc lock.
 * Stop-the-world has to deliver its signal and snapshot that stack.
 * leak() returns before the scrub, so the quarantined pointer is not
 * a live local. SIGUSR1 stays blocked except during sigsuspend, so a
 * wake that lands between the phase check and the wait stays pending. */

void filc_collect(void);
unsigned long filc_freed_count(void);

static volatile int phase;

static void on_usr1(int sig)
{
    (void)sig;
}

static void leak(void)
{
    char *p = malloc(256);
    if (!p)
        _exit(2);
    p[0] = 1;
    p[255] = 2;
    free(p);
}

static void scrub(void)
{
    volatile char buf[8192];
    int i;
    for (i = 0; i < 8192; i++)
        buf[i] = 0;
}

static void *worker(void *arg)
{
    sigset_t block, old;
    (void)arg;
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);
    if (pthread_sigmask(SIG_BLOCK, &block, &old) != 0)
        _exit(2);
    leak();
    scrub();
    phase = 1;
    while (phase == 1)
        sigsuspend(&old);
    return 0;
}

int main(void)
{
    pthread_t th;
    char *keep;
    unsigned long before;
    signal(SIGUSR1, on_usr1);
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
    filc_collect();
    if (keep[0] != 7 || keep[31] != 9)
        return 4;
    if (filc_freed_count() == before)
        return 3;
    phase = 2;
    pthread_kill(th, SIGUSR1);
    if (pthread_join(th, 0) != 0)
        return 5;
    return 0;
}
