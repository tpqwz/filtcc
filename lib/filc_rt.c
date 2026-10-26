/*
 * filc_rt.c - stage-1 runtime for TCC's optional -filc mode.
 *
 * This is a subset of Fil-C's InvisiCaps model (fil-c.org/invisicaps):
 * every object has a "lower" (the payload address) and, lazily, an aux
 * array with one capability per 8 payload bytes. The user-visible pointer
 * stays a normal 64-bit address. Integer stores do not create or clear
 * capabilities. Freed heap objects are never reused (quarantine), so a
 * stale lower cannot alias a new allocation.
 *
 * Also in this runtime: a conservative heap GC (unreachable HEAP objects
 * are returned to the system; a reachable quarantined object stays so
 * use-after-free still panics), pointer stores that update the integer
 * and the capability under one lock, signals blocked while that lock is
 * held, setjmp anchors so longjmp drops abandoned stack objects, and
 * standard-ABI entry points for indirect calls to the checked libc
 * subset (including strcpy, stpcpy, strncpy, stpncpy, strcat, strncat,
 * strnlen, strspn, strcspn, strpbrk, memcmp, memccpy, strndup, strtok,
 * strtok_r, strsep, bzero, and explicit_bzero). read, write, pread,
 * pwrite, fread, fwrite, and fgets check the buffer, then call libc;
 * a kernel transfer does not clear capabilities. getenv and
 * secure_getenv cover the returned string and leave it writable.
 * __errno_location covers the thread errno int and publishes it.
 * setenv, unsetenv, and putenv check the caller's strings. open, fopen,
 * freopen, and fdopen check their path and mode strings. puts and fputs
 * check the string. FILE* stays an opaque libc pointer. fclose, fflush,
 * fputc, putchar, close, and lseek pass their integer or FILE* bits
 * through. printf checks the format string, each %s, and each %n.
 * sprintf refuses to write past its destination object. vprintf,
 * vfprintf, vsprintf, and vsnprintf do the same checks: va_start
 * remembers which frame owns the register save area, and the walk
 * takes pointer capabilities from that frame. An indirect printf
 * walks its call frame the same way. A positional %N$ conversion
 * is not checked beyond the format string. A dynamic .* precision
 * requires a terminator instead of a byte count. mmap covers the
 * returned mapping; munmap and mprotect must
 * name that whole object. qsort is a heapsort whose swaps go through
 * filc_memmove, so an array of pointers keeps its capabilities. The
 * comparator is called with a capability frame. bsearch checks the
 * key and the array, then libc searches; a hit keeps the array's
 * capability. Nested qsort or bsearch stops at eight frames.
 * strcasecmp, strncasecmp, strcasestr, and memmem check their inputs.
 * A match keeps the haystack capability. stat, lstat, and fstat check
 * the path and the whole struct stat. strtol, strtoll, strtoul,
 * strtoull, strtof, strtod, and strtold require a terminator inside
 * the source object. The end pointer keeps that object's capability.
 * atoi, atol, atoll, and atof check the string the same way. getcwd
 * checks a caller buffer for a writable size bytes and keeps that
 * object's capability. A null buffer is the glibc extension: the
 * result is a filc heap object of size bytes, or of the path length
 * when size is 0, and the libc buffer is freed. sscanf requires a
 * terminator in the source and the format. Each storing conversion
 * checks a writable destination of that conversion's size. %s and
 * scansets need an explicit width. A positional conversion, %p, %m,
 * and a wide conversion panic instead of calling libc. fscanf and
 * scanf check the format and each destination the same way. vsscanf,
 * vfscanf, and vscanf do too: va_start remembers which frame owns the
 * register save area, and the walk takes pointer capabilities from
 * that frame, so a wrapper can pass the va_list on. va_start covers the 176-byte register
 * save area and stores that capability. The overflow cursor is left
 * alone when a stack argument is already an object there; otherwise
 * it is a zero-size capability that the first stack va_arg grows to
 * that slot. dlopen, dlsym, dlclose, and dlerror check strings and
 * handles. A handle is a zero-size capability. dlsym accepts a null
 * or (void *)-1 sentinel. A function, or resolved IFUNC code in an
 * executable segment, can be called and cannot be read. Data is
 * limited to the symbol size inside its load segment and is read-only
 * when that segment is not writable. An address with no size and no
 * executable segment cannot be read or called. dlerror covers the
 * libc message and leaves it read-only. dlclose drops the handle
 * only after that mapping is gone. Pointer
 * __atomic_load/store/exchange/compare_exchange
 * update the integer and the capability under the same lock; memory
 * order is ignored, and compare_exchange matches integer bits only.
 * Inline asm is still unchecked. A signal handler that itself
 * runs user -filc code is stopped safely only because the lock blocks
 * signals; this is not a full signal model. Aux words are stored whole.
 * TCC object addresses can be odd, so the low bit is not a flag.
 * File-scope pointer initializers arrive as filc_init records. A later
 * cover may grow an object's size and never shrinks it. Code in this
 * file is compiled without -filc.
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <setjmp.h>
#include <dlfcn.h>
#include <link.h>
#include <signal.h>
#include <ucontext.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <strings.h>
#include <sys/stat.h>

/* glibc headers #define __attribute__ to empty when __has_attribute is 0.
 * tcc still parses attributes; bcheck.c and bt-log.c undef the same way. */
#ifdef __attribute__
# undef __attribute__
#endif

#define FILC_READONLY 1
#define FILC_FUNC     2
#define FILC_HEAP     4
#define FILC_FREE     8
#define FILC_DL       16

#define FILC_MAGIC 0xF11C0B17u
#define FILC_BUCKETS 4096
#define FILC_MAX_DEPTH 128
#define FILC_MAX_PTR_ARGS 32

typedef struct filc_object {
    uint32_t magic;
    uint32_t flags;
    size_t size;
    uintptr_t *aux; /* one entry per 8 payload bytes */
    void *payload;
    struct filc_object *next;
    size_t alloc_size; /* heap bytes to charge, kept after quarantine */
    uint32_t mark;
} filc_object;

static filc_object *buckets[FILC_BUCKETS];
static pthread_mutex_t filc_mu;
static pthread_once_t filc_once = PTHREAD_ONCE_INIT;

static __thread void *call_caps[FILC_MAX_DEPTH][FILC_MAX_PTR_ARGS];
static __thread int call_depth;
static __thread int call_va_idx[FILC_MAX_DEPTH];
static __thread void *filc_ret_cap;
/* Reentry from filc_cover / fprintf while publishing errno. */
static __thread int filc_in_errno;
/* Register struct returns need one lower per pointer slot. pop does not
 * clear these; the caller zeroes the count before the call. */
#define FILC_MAX_RET_CAPS 4
static __thread void *filc_ret_caps[FILC_MAX_RET_CAPS];
static __thread int filc_ret_n;

/* One registered thread. Snapshots are filled at a safepoint; the
 * collector reads them only after ack is set. */
#define FILC_STW_SIG (SIGRTMIN + 7)
typedef struct filc_thr {
    struct filc_thr *next;
    pthread_t id;
    void *stack_lo;
    void *stack_hi;
    volatile sig_atomic_t ack;
    volatile uintptr_t sp;
    volatile int call_depth;
    volatile int ret_n;
    volatile int nregs;
    void *volatile ret_cap;
    void *volatile ret_caps[FILC_MAX_RET_CAPS];
    uintptr_t volatile regs[32];
    void *volatile caps[FILC_MAX_DEPTH][FILC_MAX_PTR_ARGS];
} filc_thr;

static filc_thr *volatile filc_threads;
static int filc_nthreads;
/* Threads bump this around first-time publication. It must be atomic:
 * every outermost lock updates it, and a lost decrement leaves stop-the-
 * world waiting forever. */
static int filc_entering;
static volatile sig_atomic_t filc_stop;
static volatile sig_atomic_t filc_stw_ok;
static int filc_inits_done;
static pthread_mutex_t filc_thr_mu;
static struct sigaction filc_old_stw;

static __thread int filc_lock_depth;
static __thread sigset_t filc_saved_mask;
static __thread int filc_is_collector;
static __thread filc_thr *filc_me;

static size_t filc_heap_bytes;
static size_t filc_gc_threshold = 256 * 1024;
static size_t filc_freed_objs;
static int filc_in_gc;

static void filc_resolve_libc(void);
static void filc_maybe_gc(void);
void filc_panic(const char *msg);

static void filc_compiler_barrier(void)
{
    __asm__ __volatile__("" : : : "memory");
}

/* tcc does not predefine __ATOMIC_SEQ_CST. A locked add is the counter
 * these paths need, and it stays correct when two threads lock at once. */
static void filc_cnt_add(int *p, int d)
{
#if defined(__x86_64__) || defined(__i386__)
    __asm__ __volatile__("lock addl %1, %0" : "+m"(*p) : "r"(d) : "memory");
#else
    __atomic_fetch_add(p, d, 5);
#endif
}

static int filc_cnt_load(int *p)
{
    int v;
#if defined(__x86_64__) || defined(__i386__)
    __asm__ __volatile__("movl %1, %0" : "=r"(v) : "m"(*p) : "memory");
#else
    v = __atomic_load_n(p, 5);
#endif
    return v;
}

static void filc_snap_tls(filc_thr *t)
{
    int i, j, depth;
    depth = call_depth;
    if (depth < 0)
        depth = 0;
    if (depth > FILC_MAX_DEPTH)
        depth = FILC_MAX_DEPTH;
    t->call_depth = depth;
    t->ret_n = filc_ret_n;
    t->ret_cap = filc_ret_cap;
    for (i = 0; i < FILC_MAX_RET_CAPS; i++)
        t->ret_caps[i] = filc_ret_caps[i];
    for (i = 0; i < depth; i++)
        for (j = 0; j < FILC_MAX_PTR_ARGS; j++)
            t->caps[i][j] = call_caps[i][j];
}

/* Record a stack bound inside this frame, then spin. The collector
 * scans [sp, stack_hi). Nothing above sp is written while we wait. */
static void filc_ack_spin(filc_thr *t)
{
    volatile char marker;
    filc_snap_tls(t);
    t->nregs = 0;
    t->sp = (uintptr_t)&marker;
    filc_compiler_barrier();
    t->ack = 1;
    while (filc_stop)
        ;
    t->ack = 0;
}

static void filc_on_stw(int sig, siginfo_t *info, void *ctx)
{
    filc_thr *t = filc_me;
    if (!filc_stop || filc_is_collector || !t) {
        if (filc_old_stw.sa_flags & SA_SIGINFO) {
            if (filc_old_stw.sa_sigaction)
                filc_old_stw.sa_sigaction(sig, info, ctx);
        } else if (filc_old_stw.sa_handler
                   && filc_old_stw.sa_handler != SIG_DFL
                   && filc_old_stw.sa_handler != SIG_IGN) {
            filc_old_stw.sa_handler(sig);
        }
        return;
    }
#ifdef __x86_64__
    if (ctx) {
        ucontext_t *uc = (ucontext_t *)ctx;
        int i, n = NGREG;
        if (n > 32)
            n = 32;
        for (i = 0; i < n; i++)
            t->regs[i] = (uintptr_t)uc->uc_mcontext.gregs[i];
        t->nregs = n;
    }
#else
    (void)ctx;
    t->nregs = 0;
#endif
    filc_ack_spin(t);
}

static void filc_publish_self(void)
{
    filc_thr *t;
    pthread_attr_t attr;
    void *base = NULL;
    size_t size = 0;
    t = (filc_thr *)calloc(1, sizeof(*t));
    if (!t)
        filc_panic("out of memory");
    t->id = pthread_self();
    if (pthread_getattr_np(pthread_self(), &attr) == 0) {
        pthread_attr_getstack(&attr, &base, &size);
        pthread_attr_destroy(&attr);
        t->stack_lo = base;
        t->stack_hi = (char *)base + size;
    }
    /* Publish under filc_thr_mu so two threads cannot drop a list node.
     * filc_entering stays nonzero across this, and stop-the-world will
     * not walk the list until that count returns to zero. */
    pthread_mutex_lock(&filc_thr_mu);
    t->next = filc_threads;
    filc_threads = t;
    filc_cnt_add(&filc_nthreads, 1);
    pthread_mutex_unlock(&filc_thr_mu);
    filc_me = t;
}

static void filc_init_lock(void)
{
    pthread_mutexattr_t a;
    struct sigaction sa;
    pthread_mutexattr_init(&a);
    pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&filc_mu, &a);
    pthread_mutex_init(&filc_thr_mu, &a);
    pthread_mutexattr_destroy(&a);
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = filc_on_stw;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    if (sigaction(FILC_STW_SIG, &sa, &filc_old_stw) == 0)
        filc_stw_ok = 1;
    filc_resolve_libc();
}

static void filc_lock(void)
{
    pthread_once(&filc_once, filc_init_lock);
    if (filc_lock_depth > 0) {
        pthread_mutex_lock(&filc_mu);
        filc_lock_depth++;
        return;
    }
    {
        sigset_t block;
        sigfillset(&block);
        sigdelset(&block, SIGSEGV);
        sigdelset(&block, SIGBUS);
        sigdelset(&block, SIGFPE);
        sigdelset(&block, SIGILL);
        sigdelset(&block, SIGABRT);
        pthread_sigmask(SIG_BLOCK, &block, &filc_saved_mask);
    }
    filc_cnt_add(&filc_entering, 1);
    if (!filc_me)
        filc_publish_self();
    filc_cnt_add(&filc_entering, -1);
    for (;;) {
        if (filc_stop && !filc_is_collector) {
            filc_ack_spin(filc_me);
            continue;
        }
        pthread_mutex_lock(&filc_mu);
        if (filc_stop && !filc_is_collector) {
            pthread_mutex_unlock(&filc_mu);
            continue;
        }
        break;
    }
    filc_lock_depth = 1;
}

static void filc_unlock(void)
{
    if (filc_lock_depth <= 0)
        return;
    filc_lock_depth--;
    pthread_mutex_unlock(&filc_mu);
    if (filc_lock_depth == 0)
        pthread_sigmask(SIG_SETMASK, &filc_saved_mask, NULL);
}

void filc_panic(const char *msg)
{
    fprintf(stderr, "filc: %s\n", msg ? msg : "panic");
    fflush(stderr);
    abort();
}

static unsigned filc_hash(void *p)
{
    uintptr_t x = (uintptr_t)p >> 3;
    x ^= x >> 16;
    x *= (uintptr_t)0x9E3779B1u;
    return (unsigned)x & (FILC_BUCKETS - 1);
}

static filc_object *filc_lookup(void *payload)
{
    filc_object *o;
    for (o = buckets[filc_hash(payload)]; o; o = o->next)
        if (o->payload == payload)
            return o;
    return NULL;
}

static void filc_insert(filc_object *o)
{
    unsigned h = filc_hash(o->payload);
    o->next = buckets[h];
    buckets[h] = o;
}

static filc_object *filc_need(void *lower, const char *what)
{
    filc_object *o;
    if (!lower)
        filc_panic("null capability");
    o = filc_lookup(lower);
    if (!o || o->payload != lower)
        filc_panic(what ? what : "bad capability");
    if (o->flags & FILC_FREE)
        filc_panic("use after free");
    return o;
}

static size_t filc_naux(filc_object *o)
{
    if (!o->size)
        return 0;
    return (o->size + sizeof(void *) - 1) / sizeof(void *);
}

static uintptr_t *filc_ensure_aux(filc_object *o)
{
    size_t n;
    if (o->aux)
        return o->aux;
    n = filc_naux(o);
    if (!n)
        filc_panic("unaligned pointer");
    o->aux = (uintptr_t *)calloc(n, sizeof(uintptr_t));
    if (!o->aux)
        filc_panic("out of memory");
    return o->aux;
}

static void filc_apply_inits(void);

static size_t filc_aux_index(filc_object *o, void *addr)
{
    uintptr_t a = (uintptr_t)addr;
    uintptr_t lo = (uintptr_t)o->payload;
    size_t idx;
    if (a & (sizeof(void *) - 1))
        filc_panic("unaligned pointer");
    if (a < lo)
        filc_panic("out of bounds");
    idx = (size_t)((a - lo) / sizeof(void *));
    if (idx >= filc_naux(o))
        filc_panic("out of bounds");
    return idx;
}

/* lower == payload. Idempotent. Stack and global objects are side
 * allocations registered in the table; heap objects are registered
 * by filc_malloc with the header immediately below the payload.
 * A second cover may set READONLY and may grow size. It does not
 * shrink, and it does not touch FREE or FUNC objects. */
void *filc_cover(void *payload, size_t size, unsigned flags)
{
    filc_object *o;
    filc_apply_inits();
    if (!payload)
        return NULL;
    filc_lock();
    o = filc_lookup(payload);
    if (o) {
        if (!(o->flags & (FILC_FREE | FILC_FUNC)) && size > o->size) {
            if (o->aux) {
                size_t old_n = filc_naux(o);
                size_t new_n;
                uintptr_t *naux;
                o->size = size;
                new_n = filc_naux(o);
                if (new_n > old_n) {
                    naux = (uintptr_t *)realloc(o->aux, new_n * sizeof(uintptr_t));
                    if (!naux) {
                        filc_unlock();
                        filc_panic("out of memory");
                    }
                    memset(naux + old_n, 0, (new_n - old_n) * sizeof(uintptr_t));
                    o->aux = naux;
                }
            } else {
                o->size = size;
            }
        }
        if ((flags & FILC_READONLY) && !(o->flags & FILC_FREE))
            o->flags |= FILC_READONLY;
        filc_unlock();
        return o->payload;
    }
    o = (filc_object *)calloc(1, sizeof(*o));
    if (!o) {
        filc_unlock();
        filc_panic("out of memory");
    }
    o->magic = FILC_MAGIC;
    o->flags = flags & ~FILC_FREE;
    o->size = size;
    o->payload = payload;
    filc_insert(o);
    filc_unlock();
    return payload;
}

void filc_uncover(void *lower)
{
    filc_object *o, **pp;
    if (!lower)
        return;
    filc_lock();
    pp = &buckets[filc_hash(lower)];
    for (; (o = *pp); pp = &o->next) {
        if (o->payload != lower)
            continue;
        if (o->flags & FILC_HEAP)
            break;
        *pp = o->next;
        free(o->aux);
        free(o);
        break;
    }
    filc_unlock();
}

void filc_check(void *lower, void *ptr, size_t size, int write)
{
    filc_object *o;
    uintptr_t p, lo, hi;
    /* Globals are registered by init records. A check can run before
     * the constructor if another constructor touches one. */
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(lower, "bad capability");
    if (write && (o->flags & FILC_READONLY)) {
        filc_unlock();
        filc_panic("write to read-only");
    }
    p = (uintptr_t)ptr;
    lo = (uintptr_t)o->payload;
    hi = lo + o->size;
    if (!ptr && size) {
        filc_unlock();
        filc_panic("null capability");
    }
    if (p < lo || p > hi || size > (size_t)(hi - p)) {
        filc_unlock();
        filc_panic("out of bounds");
    }
    filc_unlock();
}

void filc_check_func(void *lower, void *ptr)
{
    filc_object *o;
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(lower, "bad capability");
    if (!(o->flags & FILC_FUNC) || ptr != o->payload) {
        filc_unlock();
        filc_panic("not a function");
    }
    filc_unlock();
}

void *filc_load_cap(void *lower, void *addr)
{
    filc_object *o;
    uintptr_t cap = 0;
    size_t idx;
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(lower, "bad capability");
    idx = filc_aux_index(o, addr);
    if (o->aux)
        cap = o->aux[idx];
    filc_unlock();
    return (void *)cap;
}

void filc_store_cap(void *lower, void *addr, void *val_lower)
{
    filc_object *o;
    size_t idx;
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(lower, "bad capability");
    if (o->flags & FILC_READONLY) {
        filc_unlock();
        filc_panic("write to read-only");
    }
    idx = filc_aux_index(o, addr);
    if (val_lower || o->aux) {
        uintptr_t *aux = filc_ensure_aux(o);
        aux[idx] = (uintptr_t)val_lower;
    }
    filc_unlock();
}

/* One pointer initializer, or (slot == NULL) a global object's size.
 * The compiler writes this into the filc_init section. Pointers are
 * relocated; the two sizes are absolute. Keep this layout in sync with
 * FILC_INIT_REC_SIZE in tccgen.c. */
typedef struct filc_init_rec {
    void *slot;
    void *obj;
    void *tgt;
    uint64_t obj_size;
    uint64_t tgt_size;
    uint32_t obj_flags;
    uint32_t tgt_flags;
} filc_init_rec;

typedef char filc_init_rec_size_ok[(sizeof(filc_init_rec) == 48) ? 1 : -1];

extern filc_init_rec __start_filc_init[] __attribute__((weak));
extern filc_init_rec __stop_filc_init[] __attribute__((weak));

static int filc_inits_running;

static void filc_apply_inits(void)
{
    filc_init_rec *p, *end;
    if (filc_inits_done || filc_inits_running)
        return;
    filc_inits_running = 1;
    p = __start_filc_init;
    end = __stop_filc_init;
    if (p && end && p < end) {
        /* Every slot is stored before any object is frozen. A const
         * aggregate has one record per pointer; freezing after the
         * first store made the rest panic. */
        for (; p + 1 <= end; p++) {
            if (!p->obj)
                continue;
            if (!p->slot) {
                filc_cover(p->obj, (size_t)p->obj_size,
                    p->obj_flags & ~FILC_READONLY);
                continue;
            }
            filc_cover(p->obj, (size_t)p->obj_size,
                p->obj_flags & ~FILC_READONLY);
            if (p->tgt)
                filc_cover(p->tgt, (size_t)p->tgt_size,
                    p->tgt_flags & ~FILC_READONLY);
            filc_store_cap(p->obj, p->slot, p->tgt);
        }
        for (p = __start_filc_init; p + 1 <= end; p++) {
            if (!p->obj)
                continue;
            if (p->obj_flags & FILC_READONLY)
                filc_cover(p->obj, (size_t)p->obj_size, FILC_READONLY);
            if (p->slot && p->tgt && (p->tgt_flags & FILC_READONLY))
                filc_cover(p->tgt, (size_t)p->tgt_size, FILC_READONLY);
        }
    }
    filc_inits_done = 1;
    filc_inits_running = 0;
}

__attribute__((constructor)) static void filc_init_globals(void)
{
    filc_apply_inits();
}

static void *filc_alloc_heap(size_t size, int zero)
{
    size_t hdr, total;
    char *block;
    filc_object *o;
    void *payload;
    hdr = (sizeof(filc_object) + 15) & ~(size_t)15;
    if (size > (size_t)-1 - hdr)
        return NULL;
    total = hdr + size;
    block = zero ? (char *)calloc(1, total) : (char *)malloc(total);
    if (!block)
        return NULL;
    payload = block + hdr;
    o = (filc_object *)block;
    o->magic = FILC_MAGIC;
    o->flags = FILC_HEAP;
    o->size = size;
    o->aux = NULL;
    o->payload = payload;
    o->next = NULL;
    o->alloc_size = size;
    filc_lock();
    filc_heap_bytes += size;
    filc_insert(o);
    filc_unlock();
    return payload;
}

void *filc_malloc(size_t size)
{
    filc_maybe_gc();
    return filc_alloc_heap(size, 0);
}

void *filc_calloc(size_t nmemb, size_t size)
{
    if (nmemb && size > (size_t)-1 / nmemb)
        return NULL;
    filc_maybe_gc();
    return filc_alloc_heap(nmemb * size, 1);
}

void filc_free(void *lower, void *ptr)
{
    filc_object *o;
    if (!ptr)
        return;
    filc_lock();
    if (!lower) {
        filc_unlock();
        filc_panic("null capability");
    }
    o = filc_lookup(lower);
    if (!o) {
        filc_unlock();
        filc_panic("bad capability");
    }
    if (ptr != o->payload) {
        filc_unlock();
        filc_panic("free of interior pointer");
    }
    if (o->flags & FILC_FREE) {
        filc_unlock();
        filc_panic("double free");
    }
    if (!(o->flags & FILC_HEAP)) {
        filc_unlock();
        filc_panic("free of non-heap");
    }
    /* Quarantine: the header and payload are never reused or unmapped. */
    o->flags |= FILC_FREE;
    o->size = 0;
    filc_unlock();
}

void *filc_realloc(void *lower, void *ptr, size_t size)
{
    filc_object *o;
    void *np;
    size_t old, ncopy, i, na, nb;
    if (!ptr)
        return filc_malloc(size);
    filc_lock();
    o = filc_need(lower, "bad capability");
    if (ptr != o->payload) {
        filc_unlock();
        filc_panic("free of interior pointer");
    }
    if (!(o->flags & FILC_HEAP)) {
        filc_unlock();
        filc_panic("free of non-heap");
    }
    old = o->size;
    filc_unlock();
    np = filc_malloc(size);
    if (!np)
        return NULL;
    ncopy = old < size ? old : size;
    memmove(np, ptr, ncopy);
    filc_lock();
    o = filc_lookup(ptr);
    if (o && o->aux) {
        filc_object *no = filc_lookup(np);
        na = (ncopy + sizeof(void *) - 1) / sizeof(void *);
        nb = no ? filc_naux(no) : 0;
        if (na > nb)
            na = nb;
        if (no && na) {
            uintptr_t *aux = filc_ensure_aux(no);
            for (i = 0; i < na; i++)
                aux[i] = o->aux[i];
        }
    }
    if (o && (o->flags & FILC_HEAP) && !(o->flags & FILC_FREE)) {
        o->flags |= FILC_FREE;
        o->size = 0;
    }
    filc_unlock();
    return np;
}

static void filc_copy_aux(void *dl, void *d, void *sl, void *s, size_t n)
{
    filc_object *od, *os;
    uintptr_t ds, ss, a, end;
    if (!n)
        return;
    od = filc_lookup(dl);
    os = filc_lookup(sl);
    if (!od || !os || !os->aux)
        return;
    ds = (uintptr_t)d;
    ss = (uintptr_t)s;
    end = ds + n;
    a = (ds + sizeof(void *) - 1) & ~(uintptr_t)(sizeof(void *) - 1);
    for (; a + sizeof(void *) <= end; a += sizeof(void *)) {
        uintptr_t src = ss + (a - ds);
        size_t di, si;
        if (src & (sizeof(void *) - 1))
            continue;
        if ((uintptr_t)os->payload > src)
            continue;
        si = (size_t)((src - (uintptr_t)os->payload) / sizeof(void *));
        if (si >= filc_naux(os))
            continue;
        di = (size_t)((a - (uintptr_t)od->payload) / sizeof(void *));
        if (di >= filc_naux(od))
            continue;
        filc_ensure_aux(od);
        od->aux[di] = os->aux[si];
    }
}

void *filc_memcpy(void *dl, void *d, void *sl, void *s, size_t n)
{
    filc_check(dl, d, n, 1);
    filc_check(sl, s, n, 0);
    if (n && d != s)
        memcpy(d, s, n);
    filc_lock();
    filc_copy_aux(dl, d, sl, s, n);
    filc_unlock();
    filc_ret_cap = dl;
    return d;
}

void *filc_memmove(void *dl, void *d, void *sl, void *s, size_t n)
{
    void *saved = NULL;
    size_t i, count = 0;
    uintptr_t ds, a = 0, end = 0;
    filc_check(dl, d, n, 1);
    filc_check(sl, s, n, 0);
    /* Snapshot source caps before the payload move. Same-object overlap
     * would otherwise observe already-moved aux entries. */
    filc_lock();
    if (n) {
        ds = (uintptr_t)d;
        end = ds + n;
        a = (ds + sizeof(void *) - 1) & ~(uintptr_t)(sizeof(void *) - 1);
        count = (a + sizeof(void *) <= end) ? (size_t)((end - a) / sizeof(void *)) : 0;
        if (count) {
            saved = malloc(count * sizeof(void *));
            if (!saved) {
                filc_unlock();
                filc_panic("out of memory");
            }
            for (i = 0; i < count; i++) {
                void *src = (char *)s + ((a + i * sizeof(void *)) - ds);
                filc_object *os = filc_lookup(sl);
                ((void **)saved)[i] = NULL;
                if (os && os->aux && !((uintptr_t)src & (sizeof(void *) - 1))) {
                    uintptr_t so = (uintptr_t)src;
                    uintptr_t lo = (uintptr_t)os->payload;
                    if (so >= lo) {
                        size_t si = (size_t)((so - lo) / sizeof(void *));
                        if (si < filc_naux(os))
                            ((void **)saved)[i] = (void *)os->aux[si];
                    }
                }
            }
        }
    }
    filc_unlock();
    if (n)
        memmove(d, s, n);
    filc_lock();
    if (count && saved) {
        filc_object *od = filc_lookup(dl);
        if (od) {
            for (i = 0; i < count; i++) {
                void *dst = (void *)(a + i * sizeof(void *));
                size_t di;
                uintptr_t lo = (uintptr_t)od->payload;
                if ((uintptr_t)dst < lo)
                    continue;
                di = (size_t)(((uintptr_t)dst - lo) / sizeof(void *));
                if (di >= filc_naux(od))
                    continue;
                if (((void **)saved)[i] || od->aux) {
                    filc_ensure_aux(od);
                    od->aux[di] = (uintptr_t)((void **)saved)[i];
                }
            }
        }
    }
    filc_unlock();
    free(saved);
    filc_ret_cap = dl;
    return d;
}

void *filc_memset(void *dl, void *d, int c, size_t n)
{
    filc_check(dl, d, n, 1);
    if (n)
        memset(d, c, n);
    /* InvisiCaps: integer fill does not clear pointer capabilities. */
    filc_ret_cap = dl;
    return d;
}

size_t filc_strlen(void *lower, const char *s)
{
    size_t n = 0;
    for (;;) {
        filc_check(lower, (void *)(s + n), 1, 0);
        if (s[n] == 0)
            return n;
        n++;
    }
}

int filc_strcmp(void *al, const char *a, void *bl, const char *b)
{
    for (;;) {
        unsigned char ca, cb;
        filc_check(al, (void *)a, 1, 0);
        filc_check(bl, (void *)b, 1, 0);
        ca = (unsigned char)*a;
        cb = (unsigned char)*b;
        if (ca != cb || ca == 0)
            return (int)ca - (int)cb;
        a++;
        b++;
    }
}

int filc_strncmp(void *al, const char *a, void *bl, const char *b, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++) {
        unsigned char ca, cb;
        filc_check(al, (void *)(a + i), 1, 0);
        filc_check(bl, (void *)(b + i), 1, 0);
        ca = (unsigned char)a[i];
        cb = (unsigned char)b[i];
        if (ca != cb || ca == 0)
            return (int)ca - (int)cb;
    }
    return 0;
}

char *filc_strchr(void *lower, const char *s, int c)
{
    for (;;) {
        filc_check(lower, (void *)s, 1, 0);
        if (*s == (char)c) {
            filc_ret_cap = lower;
            return (char *)s;
        }
        if (*s == 0) {
            filc_ret_cap = NULL;
            return NULL;
        }
        s++;
    }
}

char *filc_strrchr(void *lower, const char *s, int c)
{
    const char *hit = NULL;
    for (;;) {
        filc_check(lower, (void *)s, 1, 0);
        if (*s == (char)c)
            hit = s;
        if (*s == 0)
            break;
        s++;
    }
    filc_ret_cap = hit ? lower : NULL;
    return (char *)hit;
}

void *filc_memchr(void *lower, const void *s, int c, size_t n)
{
    const unsigned char *p = (const unsigned char *)s;
    size_t i;
    for (i = 0; i < n; i++) {
        filc_check(lower, (void *)(p + i), 1, 0);
        if (p[i] == (unsigned char)c) {
            filc_ret_cap = lower;
            return (void *)(p + i);
        }
    }
    filc_ret_cap = NULL;
    return NULL;
}

char *filc_strstr(void *hl, const char *h, void *nl, const char *n)
{
    size_t nlen;
    if (!n || !h) {
        filc_ret_cap = NULL;
        return NULL;
    }
    nlen = filc_strlen(nl, n);
    if (nlen == 0) {
        filc_ret_cap = hl;
        return (char *)h;
    }
    for (;;) {
        size_t i;
        filc_check(hl, (void *)h, 1, 0);
        if (*h == 0) {
            filc_ret_cap = NULL;
            return NULL;
        }
        for (i = 0; i < nlen; i++) {
            filc_check(hl, (void *)(h + i), 1, 0);
            filc_check(nl, (void *)(n + i), 1, 0);
            if (h[i] != n[i])
                break;
        }
        if (i == nlen) {
            filc_ret_cap = hl;
            return (char *)h;
        }
        h++;
    }
}

char *filc_strdup(void *lower, const char *s)
{
    size_t n = filc_strlen(lower, s);
    char *d = (char *)filc_malloc(n + 1);
    if (!d)
        return NULL;
    memcpy(d, s, n + 1);
    /* contents are bytes; no pointer caps to copy */
    return d;
}

static void filc_copy_range(void *dl, void *d, void *sl, void *s, size_t n)
{
    filc_lock();
    filc_copy_aux(dl, d, sl, s, n);
    filc_unlock();
}

/* n includes the terminator when the source had one. Overflow of the
 * checked length is reported as out of bounds rather than a wrapped size. */
static size_t filc_need_room(size_t have, size_t add)
{
    if (add > (size_t)-1 - have)
        filc_panic("out of bounds");
    return have + add;
}

char *filc_strcpy(void *dl, char *d, void *sl, const char *s)
{
    size_t n = filc_strlen(sl, s);
    filc_need_room(n, 1);
    filc_check(dl, d, n + 1, 1);
    memcpy(d, s, n + 1);
    filc_copy_range(dl, d, sl, (void *)s, n + 1);
    filc_ret_cap = dl;
    return d;
}

char *filc_strncpy(void *dl, char *d, void *sl, const char *s, size_t n)
{
    size_t i, copied = 0;
    filc_check(dl, d, n, 1);
    for (i = 0; i < n; i++) {
        filc_check(sl, (void *)(s + i), 1, 0);
        d[i] = s[i];
        copied = i + 1;
        if (s[i] == 0)
            break;
    }
    /* The pad is an integer fill. It does not clear capabilities. */
    for (; i < n; i++)
        d[i] = 0;
    filc_copy_range(dl, d, sl, (void *)s, copied);
    filc_ret_cap = dl;
    return d;
}

char *filc_strcat(void *dl, char *d, void *sl, const char *s)
{
    size_t dn = filc_strlen(dl, d);
    size_t sn = filc_strlen(sl, s);
    filc_need_room(dn, sn + 1);
    filc_check(dl, d + dn, sn + 1, 1);
    memcpy(d + dn, s, sn + 1);
    filc_copy_range(dl, d + dn, sl, (void *)s, sn + 1);
    filc_ret_cap = dl;
    return d;
}

char *filc_strncat(void *dl, char *d, void *sl, const char *s, size_t n)
{
    size_t dn, sn, i;
    dn = filc_strlen(dl, d);
    sn = 0;
    for (i = 0; i < n; i++) {
        filc_check(sl, (void *)(s + i), 1, 0);
        if (s[i] == 0)
            break;
        sn++;
    }
    filc_need_room(dn, sn + 1);
    filc_check(dl, d + dn, sn + 1, 1);
    if (sn)
        memcpy(d + dn, s, sn);
    d[dn + sn] = 0;
    filc_copy_range(dl, d + dn, sl, (void *)s, sn);
    filc_ret_cap = dl;
    return d;
}

size_t filc_strnlen(void *lower, const char *s, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++) {
        filc_check(lower, (void *)(s + i), 1, 0);
        if (s[i] == 0)
            return i;
    }
    return n;
}

int filc_memcmp(void *al, const void *a, void *bl, const void *b, size_t n)
{
    filc_check(al, (void *)a, n, 0);
    filc_check(bl, (void *)b, n, 0);
    if (!n)
        return 0;
    return memcmp(a, b, n);
}

char *filc_strndup(void *lower, const char *s, size_t n)
{
    size_t len = filc_strnlen(lower, s, n);
    char *d;
    filc_need_room(len, 1);
    d = (char *)filc_malloc(len + 1);
    if (!d)
        return NULL;
    if (len)
        memcpy(d, s, len);
    d[len] = 0;
    return d;
}

/* Return the pointer to the written nul. The capability is still dest. */
char *filc_stpcpy(void *dl, char *d, void *sl, const char *s)
{
    size_t n = filc_strlen(sl, s);
    filc_strcpy(dl, d, sl, s);
    return d + n;
}

/* POSIX: pointer to the first written nul, or dest+n if none was written.
 * The pad zeros are an integer fill and do not clear capabilities. */
char *filc_stpncpy(void *dl, char *d, void *sl, const char *s, size_t n)
{
    size_t i, copied = 0;
    char *ret = d + n;
    filc_check(dl, d, n, 1);
    for (i = 0; i < n; i++) {
        filc_check(sl, (void *)(s + i), 1, 0);
        d[i] = s[i];
        copied = i + 1;
        if (s[i] == 0) {
            ret = d + i;
            break;
        }
    }
    for (; i < n; i++)
        d[i] = 0;
    filc_copy_range(dl, d, sl, (void *)s, copied);
    filc_ret_cap = dl;
    return ret;
}

static int filc_in_set(void *al, const char *acc, unsigned char ch)
{
    for (;;) {
        filc_check(al, (void *)acc, 1, 0);
        if (*acc == 0)
            return 0;
        if ((unsigned char)*acc == ch)
            return 1;
        acc++;
    }
}

size_t filc_strspn(void *sl, const char *s, void *al, const char *acc)
{
    size_t n = 0;
    for (;;) {
        unsigned char ch;
        filc_check(sl, (void *)(s + n), 1, 0);
        ch = (unsigned char)s[n];
        if (ch == 0 || !filc_in_set(al, acc, ch))
            return n;
        n++;
    }
}

size_t filc_strcspn(void *sl, const char *s, void *al, const char *rej)
{
    size_t n = 0;
    for (;;) {
        unsigned char ch;
        filc_check(sl, (void *)(s + n), 1, 0);
        ch = (unsigned char)s[n];
        if (ch == 0 || filc_in_set(al, rej, ch))
            return n;
        n++;
    }
}

char *filc_strpbrk(void *sl, const char *s, void *al, const char *acc)
{
    size_t n = 0;
    for (;;) {
        unsigned char ch;
        filc_check(sl, (void *)(s + n), 1, 0);
        ch = (unsigned char)s[n];
        if (ch == 0) {
            filc_ret_cap = NULL;
            return NULL;
        }
        if (filc_in_set(al, acc, ch)) {
            filc_ret_cap = sl;
            return (char *)(s + n);
        }
        n++;
    }
}

void *filc_memccpy(void *dl, void *d, void *sl, const void *s, int c, size_t n)
{
    unsigned char *dd = (unsigned char *)d;
    const unsigned char *ss = (const unsigned char *)s;
    size_t i;
    if (n == 0) {
        filc_check(dl, d, 0, 1);
        filc_check(sl, (void *)s, 0, 0);
        filc_ret_cap = NULL;
        return NULL;
    }
    for (i = 0; i < n; i++) {
        filc_check(sl, (void *)(ss + i), 1, 0);
        filc_check(dl, dd + i, 1, 1);
        dd[i] = ss[i];
        if (ss[i] == (unsigned char)c) {
            filc_copy_range(dl, d, sl, (void *)s, i + 1);
            filc_ret_cap = dl;
            return dd + i + 1;
        }
    }
    filc_copy_range(dl, d, sl, (void *)s, n);
    filc_ret_cap = NULL;
    return NULL;
}

/* Integer fill of zeros. Capabilities stay, same as memset(0). */
void filc_explicit_bzero(void *dl, void *d, size_t n)
{
    filc_memset(dl, d, 0, n);
}

/* Scan one token. collapse skips leading delimiters (strtok). Otherwise
 * a leading delimiter is an empty token (strsep). *next is the resume
 * pointer, or NULL when the string is exhausted. A delimiter is replaced
 * with a nul; that write is checked. */
static char *filc_split_tok(void *sl, char *s, void *dl, const char *delim,
    int collapse, char **next)
{
    unsigned char ch;
    char *tok;
    if (!s) {
        *next = NULL;
        return NULL;
    }
    if (collapse) {
        for (;;) {
            filc_check(sl, s, 1, 0);
            ch = (unsigned char)*s;
            if (ch == 0) {
                *next = NULL;
                return NULL;
            }
            if (!filc_in_set(dl, delim, ch))
                break;
            s++;
        }
    }
    tok = s;
    for (;;) {
        filc_check(sl, s, 1, 0);
        ch = (unsigned char)*s;
        if (ch == 0) {
            *next = NULL;
            return tok;
        }
        if (filc_in_set(dl, delim, ch)) {
            filc_check(sl, s, 1, 1);
            *s = 0;
            *next = s + 1;
            return tok;
        }
        s++;
    }
}

static __thread char *filc_tok_s;
static __thread void *filc_tok_cap;

void filc_store_ptr(void *obj_lower, void *addr, void *val_lower, uintptr_t intval);

char *filc_strtok(void *sl, char *str, void *dl, const char *delim)
{
    char *next, *tok;
    if (!str) {
        str = filc_tok_s;
        sl = filc_tok_cap;
    }
    tok = filc_split_tok(sl, str, dl, delim, 1, &next);
    filc_tok_s = next;
    filc_tok_cap = next ? sl : NULL;
    filc_ret_cap = tok ? sl : NULL;
    return tok;
}

char *filc_strtok_r(void *sl, char *str, void *dl, const char *delim,
    void *pl, char **saveptr)
{
    char *next, *tok;
    if (!str) {
        filc_check(pl, saveptr, sizeof(char *), 0);
        str = *saveptr;
        sl = filc_load_cap(pl, saveptr);
    }
    tok = filc_split_tok(sl, str, dl, delim, 1, &next);
    if (next)
        filc_store_ptr(pl, saveptr, sl, (uintptr_t)next);
    else
        filc_store_ptr(pl, saveptr, NULL, 0);
    filc_ret_cap = tok ? sl : NULL;
    return tok;
}

char *filc_strsep(void *pl, char **stringp, void *dl, const char *delim)
{
    char *s, *next, *tok;
    void *sl;
    filc_check(pl, stringp, sizeof(char *), 0);
    s = *stringp;
    if (!s) {
        filc_ret_cap = NULL;
        return NULL;
    }
    sl = filc_load_cap(pl, stringp);
    tok = filc_split_tok(sl, s, dl, delim, 0, &next);
    if (next)
        filc_store_ptr(pl, stringp, sl, (uintptr_t)next);
    else
        filc_store_ptr(pl, stringp, NULL, 0);
    filc_ret_cap = tok ? sl : NULL;
    return tok;
}

void filc_call_push(void)
{
    if (call_depth >= FILC_MAX_DEPTH)
        filc_panic("call depth");
    memset(call_caps[call_depth], 0, sizeof call_caps[0]);
    call_va_idx[call_depth] = 0;
    call_depth++;
}

void filc_call_set(int idx, void *lower)
{
    if (call_depth <= 0)
        filc_panic("no call frame");
    if (idx < 0 || idx >= FILC_MAX_PTR_ARGS)
        filc_panic("too many pointer args");
    call_caps[call_depth - 1][idx] = lower;
}

void *filc_call_get(int idx)
{
    /* A foreign caller (the CRT, pthread, a signal) has no capability
     * frame. Pointer parameters then carry a null lower. Using one
     * panics at the check, which is safer than inventing a capability. */
    if (call_depth <= 0 || idx < 0 || idx >= FILC_MAX_PTR_ARGS)
        return NULL;
    return call_caps[call_depth - 1][idx];
}

void filc_call_pop(void)
{
    if (call_depth <= 0)
        filc_panic("call underflow");
    call_depth--;
    /* filc_ret_cap is intentionally left alone */
}

void filc_set_ret(void *lower)
{
    filc_ret_cap = lower;
}

void *filc_get_ret(void)
{
    return filc_ret_cap;
}

void filc_set_ret_n(int n)
{
    int i;
    if (n < 0 || n > FILC_MAX_RET_CAPS)
        filc_panic("too many return caps");
    for (i = 0; i < FILC_MAX_RET_CAPS; i++)
        filc_ret_caps[i] = NULL;
    filc_ret_n = n;
}

void filc_set_ret_at(int idx, void *lower)
{
    if (idx < 0 || idx >= filc_ret_n)
        filc_panic("return cap index");
    filc_ret_caps[idx] = lower;
}

void *filc_get_ret_at(int idx)
{
    if (idx < 0 || idx >= filc_ret_n)
        return NULL;
    return filc_ret_caps[idx];
}

/* Cover argv and each argument string. The returned lower is the argv
 * array itself (lower == payload). Called from main, which has no
 * caller capability frame. */
/* Stack objects are tracked per thread so a function can drop every
 * object it covered, including ones created on untaken branches (those
 * were never tracked). The mark is the depth at function entry. */
static __thread void **track_stk;
static __thread int track_n, track_cap;

void filc_track(void *lower)
{
    int i;
    if (!lower)
        return;
    /* Loops re-cover the same stack address. One entry is enough. */
    for (i = 0; i < track_n; i++)
        if (track_stk[i] == lower)
            return;
    if (track_n >= track_cap) {
        int nc = track_cap ? track_cap * 2 : 32;
        void **p = (void **)realloc(track_stk, (size_t)nc * sizeof(void *));
        if (!p)
            filc_panic("out of memory");
        track_stk = p;
        track_cap = nc;
    }
    track_stk[track_n++] = lower;
}

void *filc_track_mark(void)
{
    return (void *)(uintptr_t)track_n;
}

/* Backward goto reads a mark that is stored only when the label runs.
 * A switch can jump to `goto L` before `L:` has executed, so the mark
 * cannot live in an uninitialized stack slot. The key is the address of
 * that slot; missing keys are not visited yet. */
static __thread struct filc_lab {
    void *key;
    int mark;
} *filc_labs;
static __thread int filc_nlabs, filc_labcap;

void filc_label_set(void *key)
{
    int i;
    struct filc_lab *p;
    if (!key)
        return;
    for (i = 0; i < filc_nlabs; i++) {
        if (filc_labs[i].key == key) {
            filc_labs[i].mark = track_n;
            return;
        }
    }
    if (filc_nlabs >= filc_labcap) {
        int nc = filc_labcap ? filc_labcap * 2 : 32;
        p = (struct filc_lab *)realloc(filc_labs, (size_t)nc * sizeof(*p));
        if (!p)
            filc_panic("out of memory");
        filc_labs = p;
        filc_labcap = nc;
    }
    filc_labs[filc_nlabs].key = key;
    filc_labs[filc_nlabs].mark = track_n;
    filc_nlabs++;
}

void filc_untrack_to(void *mark);

void filc_label_untrack(void *key)
{
    int i;
    if (!key)
        return;
    for (i = 0; i < filc_nlabs; i++) {
        if (filc_labs[i].key == key) {
            filc_untrack_to((void *)(uintptr_t)filc_labs[i].mark);
            return;
        }
    }
}

/* Drop marks whose slot is in this frame or a deeper one. The caller's
 * frame pointer is above those slots. */
void filc_label_clear(void)
{
    char *fp = (char *)__builtin_frame_address(1);
    int i = 0;
    while (i < filc_nlabs) {
        if ((char *)filc_labs[i].key < fp) {
            filc_labs[i] = filc_labs[--filc_nlabs];
            continue;
        }
        i++;
    }
}

/* A label records the track depth at which it last ran. A goto to an
 * earlier label drops objects tracked after that earlier label, so a
 * later label's saved depth can sit above track_n. That depth is already
 * passed: clamp it. A depth above track_n must not drop every object. */
static void filc_clamp_lab_marks(void)
{
    int i;
    for (i = 0; i < filc_nlabs; i++) {
        if (filc_labs[i].mark > track_n)
            filc_labs[i].mark = track_n;
    }
}

void filc_untrack_to(void *mark)
{
    int m = (int)(uintptr_t)mark;
    /* m == 0 drops everything tracked so far. A negative or a depth
     * above the stack is a stale label, not a request to do that. */
    if (m < 0)
        return;
    if (m > track_n)
        m = track_n;
    while (track_n > m) {
        void *lower = track_stk[--track_n];
        filc_uncover(lower);
    }
    filc_clamp_lab_marks();
}

void *filc_enter_main(int argc, char **argv)
{
    void *lower;
    int i;
    if (!argv || argc < 0)
        return NULL;
    lower = filc_cover(argv, (size_t)(argc + 1) * sizeof(char *), 0);
    for (i = 0; i < argc; i++) {
        char *s = argv[i];
        void *sl = NULL;
        if (s)
            sl = filc_cover(s, strlen(s) + 1, FILC_READONLY);
        filc_store_cap(lower, &argv[i], sl);
    }
    filc_store_cap(lower, &argv[argc], NULL);
    return lower;
}

/* Drop tracked stack objects whose payload is below the caller's stack
 * pointer. Used at labels, after the stack pointer has been restored.
 * x86_64: call; push rbp; mov rbp, rsp makes the caller's SP equal FP+16.
 * Heap and function objects are not tracked. */
void filc_untrack_below(void)
{
    void *fp = __builtin_frame_address(0);
    char *limit;
    int i = 0;
#ifdef __x86_64__
    limit = (char *)fp + 16;
#else
    limit = (char *)fp;
#endif
    while (i < track_n) {
        void *lower = track_stk[i];
        filc_object *o;
        int drop = 0;
        filc_lock();
        o = filc_lookup(lower);
        if (o && !(o->flags & (FILC_HEAP | FILC_FUNC))
            && (char *)o->payload < limit)
            drop = 1;
        filc_unlock();
        if (!drop) {
            i++;
            continue;
        }
        filc_uncover(lower);
        track_stk[i] = track_stk[--track_n];
    }
    /* A dropped entry can leave a label depth above the new track_n. */
    filc_clamp_lab_marks();
}

#define FILC_MAX_JMP 96
static struct {
    void *buf;
    int mark;
} filc_jmps[FILC_MAX_JMP];
static int filc_njmp;

void filc_setjmp_anchor(void *lower, void *ptr, int kind)
{
    size_t sz;
    int i;
    sz = kind ? sizeof(sigjmp_buf) : sizeof(jmp_buf);
    filc_check(lower, ptr, sz, 1);
    filc_lock();
    for (i = 0; i < filc_njmp; i++) {
        if (filc_jmps[i].buf == ptr) {
            filc_jmps[i].mark = track_n;
            filc_unlock();
            return;
        }
    }
    if (filc_njmp >= FILC_MAX_JMP) {
        filc_unlock();
        filc_panic("too many setjmp");
    }
    filc_jmps[filc_njmp].buf = ptr;
    filc_jmps[filc_njmp].mark = track_n;
    filc_njmp++;
    filc_unlock();
}

typedef void (*filc_lj_fn)(void *, int);
static filc_lj_fn real_longjmp, real__longjmp, real_siglongjmp;

static void filc_do_longjmp(filc_lj_fn real, void *buf, int val, const char *what)
{
    int i, mark = -1;
    filc_lock();
    for (i = 0; i < filc_njmp; i++) {
        if (filc_jmps[i].buf == buf) {
            mark = filc_jmps[i].mark;
            break;
        }
    }
    filc_unlock();
    if (mark >= 0)
        filc_untrack_to((void *)(uintptr_t)mark);
    if (!real)
        filc_panic(what);
    real(buf, val);
}

void filc_longjmp(void *buf, int val)
{
    filc_do_longjmp(real_longjmp, buf, val, "longjmp");
}

void filc__longjmp(void *buf, int val)
{
    filc_do_longjmp(real__longjmp, buf, val, "_longjmp");
}

void filc_siglongjmp(void *buf, int val)
{
    filc_do_longjmp(real_siglongjmp, buf, val, "siglongjmp");
}

/* Pointer store: aux word and integer bits under one lock. An 8-byte
 * slot must lie entirely inside the object; aux_index alone allows a
 * pointer that hangs off a short tail. */
void filc_store_ptr(void *obj_lower, void *addr, void *val_lower, uintptr_t intval)
{
    filc_object *o;
    uintptr_t a, lo, hi;
    size_t idx;
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(obj_lower, "bad capability");
    if (o->flags & FILC_READONLY) {
        filc_unlock();
        filc_panic("write to read-only");
    }
    a = (uintptr_t)addr;
    lo = (uintptr_t)o->payload;
    hi = lo + o->size;
    if (a & (sizeof(void *) - 1)) {
        filc_unlock();
        filc_panic("unaligned pointer");
    }
    if (a < lo || a > hi || sizeof(void *) > (size_t)(hi - a)) {
        filc_unlock();
        filc_panic("out of bounds");
    }
    idx = (size_t)((a - lo) / sizeof(void *));
    if (val_lower || o->aux) {
        uintptr_t *aux = filc_ensure_aux(o);
        aux[idx] = (uintptr_t)val_lower;
    }
    *(uintptr_t *)addr = intval;
    filc_unlock();
}

/* The caller holds filc_mu. A failure unlocks before it panics. */
static void filc_ptr_idx(filc_object *o, void *addr, size_t *idx)
{
    uintptr_t a, lo, hi;
    a = (uintptr_t)addr;
    lo = (uintptr_t)o->payload;
    hi = lo + o->size;
    if (a & (sizeof(void *) - 1)) {
        filc_unlock();
        filc_panic("unaligned pointer");
    }
    if (a < lo || a > hi || sizeof(void *) > (size_t)(hi - a)) {
        filc_unlock();
        filc_panic("out of bounds");
    }
    *idx = (size_t)((a - lo) / sizeof(void *));
}

static void filc_reject_ro(filc_object *o)
{
    if (o->flags & FILC_READONLY) {
        filc_unlock();
        filc_panic("write to read-only");
    }
}

/* Mutex atomicity for a pointer slot. Not a CPU wait-free atomic.
 * The loaded capability is published in filc_ret_cap. */
void *filc_atomic_load(void *lower, void *addr)
{
    filc_object *o;
    size_t idx;
    uintptr_t val;
    filc_lock();
    o = filc_need(lower, "bad capability");
    filc_ptr_idx(o, addr, &idx);
    val = *(uintptr_t *)addr;
    filc_ret_cap = o->aux ? (void *)o->aux[idx] : NULL;
    filc_unlock();
    return (void *)val;
}

void *filc_atomic_xchg(void *lower, void *addr, void *val_lower, uintptr_t intval)
{
    filc_object *o;
    size_t idx;
    uintptr_t old;
    void *old_cap;
    filc_lock();
    o = filc_need(lower, "bad capability");
    filc_reject_ro(o);
    filc_ptr_idx(o, addr, &idx);
    old_cap = o->aux ? (void *)o->aux[idx] : NULL;
    old = *(uintptr_t *)addr;
    if (val_lower || o->aux) {
        uintptr_t *aux = filc_ensure_aux(o);
        aux[idx] = (uintptr_t)val_lower;
    }
    *(uintptr_t *)addr = intval;
    filc_ret_cap = old_cap;
    filc_unlock();
    return (void *)old;
}

/* Compare the integer bits only. On mismatch, *exp_ptr receives the
 * current integer and its capability. */
int filc_atomic_cmpxchg(void *obj_lower, void *addr, void *exp_lower,
                        void *exp_ptr, void *des_lower, uintptr_t desired)
{
    filc_object *o, *e;
    size_t idx, eidx;
    uintptr_t cur, expv;
    void *cur_cap;
    filc_lock();
    o = filc_need(obj_lower, "bad capability");
    filc_ptr_idx(o, addr, &idx);
    e = filc_need(exp_lower, "bad capability");
    filc_ptr_idx(e, exp_ptr, &eidx);
    cur = *(uintptr_t *)addr;
    expv = *(uintptr_t *)exp_ptr;
    cur_cap = o->aux ? (void *)o->aux[idx] : NULL;
    if (cur == expv) {
        filc_reject_ro(o);
        if (des_lower || o->aux) {
            uintptr_t *aux = filc_ensure_aux(o);
            aux[idx] = (uintptr_t)des_lower;
        }
        *(uintptr_t *)addr = desired;
        filc_unlock();
        return 1;
    }
    filc_reject_ro(e);
    if (cur_cap || e->aux) {
        uintptr_t *aux = filc_ensure_aux(e);
        aux[eidx] = (uintptr_t)cur_cap;
    }
    *(uintptr_t *)exp_ptr = cur;
    filc_unlock();
    return 0;
}

void filc_va_cursor(int idx)
{
    if (call_depth <= 0)
        filc_panic("no call frame");
    if (idx < 0)
        idx = 0;
    if (idx > FILC_MAX_PTR_ARGS)
        idx = FILC_MAX_PTR_ARGS;
    call_va_idx[call_depth - 1] = idx;
}

void *filc_va_take(void)
{
    int idx;
    void *cap;
    if (call_depth <= 0)
        return NULL;
    idx = call_va_idx[call_depth - 1];
    if (idx < 0 || idx >= FILC_MAX_PTR_ARGS)
        return NULL;
    cap = call_caps[call_depth - 1][idx];
    call_va_idx[call_depth - 1] = idx + 1;
    return cap;
}

/* 1 if the compiler should install variadic caps.
 * 0 means a stack copy already carries aux; do not take.
 * A zero-size object is the overflow cursor from filc_va_start: grow
 * it to this slot. A larger object is the register save area, whose
 * base slot still needs caps. Do not cover a second object there. */
int filc_va_prepare(void *ptr, size_t size)
{
    filc_object *o;
    if (!ptr)
        filc_panic("null capability");
    filc_lock();
    o = filc_lookup(ptr);
    filc_unlock();
    if (o && o->size == 0) {
        filc_cover(ptr, size, 0);
        return 1;
    }
    if (o && o->size <= size)
        return 0;
    if (!o) {
        filc_cover(ptr, size, 0);
        filc_track(ptr);
    }
    return 1;
}

/* x86_64 SysV register save area: 6 integer regs * 8 + 8 xmm * 16.
 * The prologue builds it at rbp-200. Offsets in the 24-byte template:
 * overflow_arg_area at 8, reg_save_area at 16. */
#define FILC_VA_SAVE 176

/* The variadic frame owns the pointer caps. A later vsscanf may run
 * in a wrapper, so remember that frame by the save-area address the
 * va_list still holds. call_va_idx of that frame stays the cursor
 * va_arg already uses. */
struct filc_va_bind {
    void *save;
    int depth;
};
static __thread struct filc_va_bind *filc_va_binds;
static __thread int filc_va_nbinds, filc_va_bindcap;

static void filc_va_bind(void *save)
{
    int depth, i;
    struct filc_va_bind *p;
    if (!save)
        return;
    depth = call_depth - 1;
    if (depth < 0)
        return;
    i = 0;
    while (i < filc_va_nbinds) {
        if (filc_va_binds[i].depth >= call_depth) {
            filc_va_binds[i] = filc_va_binds[--filc_va_nbinds];
            continue;
        }
        i++;
    }
    for (i = 0; i < filc_va_nbinds; i++) {
        if (filc_va_binds[i].save == save) {
            filc_va_binds[i].depth = depth;
            return;
        }
    }
    if (filc_va_nbinds >= filc_va_bindcap) {
        int nc = filc_va_bindcap ? filc_va_bindcap * 2 : 16;
        p = (struct filc_va_bind *)realloc(filc_va_binds,
            (size_t)nc * sizeof(*p));
        if (!p)
            filc_panic("out of memory");
        filc_va_binds = p;
        filc_va_bindcap = nc;
    }
    filc_va_binds[filc_va_nbinds].save = save;
    filc_va_binds[filc_va_nbinds].depth = depth;
    filc_va_nbinds++;
}

static void *filc_take_save(void *save)
{
    int i, depth, idx;
    if (!save)
        return NULL;
    for (i = 0; i < filc_va_nbinds; i++) {
        if (filc_va_binds[i].save != save)
            continue;
        depth = filc_va_binds[i].depth;
        if (depth < 0 || depth >= call_depth)
            return NULL;
        idx = call_va_idx[depth];
        if (idx < 0 || idx >= FILC_MAX_PTR_ARGS)
            return NULL;
        call_va_idx[depth] = idx + 1;
        return call_caps[depth][idx];
    }
    return NULL;
}

/* Copy the 24-byte SysV va_list template from the caller's frame.
 * The variadic prologue stores it at rbp-24. reg_save_area becomes a
 * real capability so an instrumented __va_arg can walk it. The overflow
 * area has no stored length. Covering a span would grow an argument
 * object that already starts there, or run into the caller's frame.
 * A missing object gets a zero-size cursor; filc_va_prepare grows the
 * first slot. __va_arg advances that cursor and does not dereference it. */
void filc_va_start(void *lower, void *ap)
{
    char *src = (char *)__builtin_frame_address(1) - 24;
    char *save, *ov;
    filc_object *o;
    filc_check(lower, ap, 24, 1);
    memcpy(ap, src, 24);
    save = *(char **)((char *)ap + 16);
    if (save) {
        filc_cover(save, FILC_VA_SAVE, 0);
        filc_track(save);
        filc_store_ptr(lower, (char *)ap + 16, save, (uintptr_t)save);
        filc_va_bind(save);
    }
    ov = *(char **)((char *)ap + 8);
    if (!ov)
        return;
    filc_lock();
    o = filc_lookup(ov);
    filc_unlock();
    if (!o) {
        filc_cover(ov, 0, 0);
        filc_track(ov);
    }
    filc_store_ptr(lower, (char *)ap + 8, ov, (uintptr_t)ov);
}

/* size*nmemb, or panic. The product is only the checked length; libc
 * still receives the original factors. */
static size_t filc_io_bytes(size_t size, size_t nmemb)
{
    if (size && nmemb > (size_t)-1 / size)
        filc_panic("out of bounds");
    return size * nmemb;
}

ssize_t filc_read(void *lower, int fd, void *buf, size_t count)
{
    filc_check(lower, buf, count, 1);
    return read(fd, buf, count);
}

ssize_t filc_write(void *lower, int fd, const void *buf, size_t count)
{
    filc_check(lower, (void *)buf, count, 0);
    return write(fd, buf, count);
}

ssize_t filc_pread(void *lower, int fd, void *buf, size_t count, off_t offset)
{
    filc_check(lower, buf, count, 1);
    return pread(fd, buf, count, offset);
}

ssize_t filc_pwrite(void *lower, int fd, const void *buf, size_t count,
    off_t offset)
{
    filc_check(lower, (void *)buf, count, 0);
    return pwrite(fd, buf, count, offset);
}

size_t filc_fread(void *lower, void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    filc_check(lower, ptr, filc_io_bytes(size, nmemb), 1);
    return fread(ptr, size, nmemb, stream);
}

size_t filc_fwrite(void *lower, const void *ptr, size_t size, size_t nmemb,
    FILE *stream)
{
    filc_check(lower, (void *)ptr, filc_io_bytes(size, nmemb), 0);
    return fwrite(ptr, size, nmemb, stream);
}

/* fgets may return an interior pointer. The capability stays the buffer. */
char *filc_fgets(void *lower, char *s, int size, FILE *stream)
{
    char *r;
    if (size < 0)
        filc_panic("out of bounds");
    filc_check(lower, s, (size_t)size, 1);
    r = fgets(s, size, stream);
    filc_ret_cap = r ? lower : NULL;
    return r;
}

/* A caller buffer must be writable for size bytes. The glibc extension
 * getcwd(NULL, size) mallocs; that pointer would be foreign, so copy it
 * into a filc heap object of the same size (or the path length if size
 * is 0) and free the libc buffer. */
char *filc_getcwd(void *lower, char *buf, size_t size)
{
    char *r;
    if (!buf) {
        size_t n;
        char *d;
        r = getcwd(NULL, size);
        if (!r) {
            filc_ret_cap = NULL;
            return NULL;
        }
        n = size ? size : strlen(r) + 1;
        d = filc_malloc(n);
        if (!d) {
            free(r);
            filc_ret_cap = NULL;
            return NULL;
        }
        memcpy(d, r, n);
        free(r);
        filc_ret_cap = d;
        return d;
    }
    filc_check(lower, buf, size, 1);
    r = getcwd(buf, size);
    filc_ret_cap = r ? lower : NULL;
    return r;
}

/* A dlopen handle is not a byte object and not a heap object. free()
 * must not release linker memory, and the collector does not sweep it.
 * The flag is set here because filc_cover does not add flags to an
 * object it already knows. */
struct filc_seg {
    uintptr_t addr;
    uintptr_t hi;
    int found;
    int writable;
    int exec;
};

static int filc_seg_cb(struct dl_phdr_info *info, size_t sz, void *data)
{
    struct filc_seg *s = data;
    int i;
    (void)sz;
    for (i = 0; i < (int)info->dlpi_phnum; i++) {
        const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        uintptr_t lo, hi;
        if (ph->p_type != PT_LOAD)
            continue;
        lo = (uintptr_t)info->dlpi_addr + (uintptr_t)ph->p_vaddr;
        hi = lo + (uintptr_t)ph->p_memsz;
        if (s->addr >= lo && s->addr < hi) {
            s->found = 1;
            s->writable = (ph->p_flags & PF_W) != 0;
            s->exec = (ph->p_flags & PF_X) != 0;
            s->hi = hi;
            return 1;
        }
    }
    return 0;
}

struct filc_base {
    uintptr_t base;
    int found;
};

static int filc_base_cb(struct dl_phdr_info *info, size_t sz, void *data)
{
    struct filc_base *b = data;
    (void)sz;
    if ((uintptr_t)info->dlpi_addr == b->base) {
        b->found = 1;
        return 1;
    }
    return 0;
}

static int filc_mapping_present(uintptr_t base)
{
    struct filc_base b;
    b.base = base;
    b.found = 0;
    dl_iterate_phdr(filc_base_cb, &b);
    return b.found;
}

static void filc_dl_handle(void *lower, void *ptr)
{
    filc_object *o;
    if (!ptr || lower != ptr)
        filc_panic("bad capability");
    filc_lock();
    o = filc_lookup(ptr);
    filc_unlock();
    if (!o || o->payload != ptr || (o->flags & FILC_FREE) ||
        !(o->flags & FILC_DL))
        filc_panic("bad capability");
}

static void filc_publish_func(void *p)
{
    filc_object *o;
    filc_lock();
    o = filc_lookup(p);
    filc_unlock();
    if (o && (o->flags & FILC_FREE)) {
        filc_ret_cap = NULL;
        return;
    }
    if (!o)
        filc_cover(p, 0, FILC_FUNC);
    filc_ret_cap = p;
}

/* base is the symbol address. An existing object is left alone so a
 * -filc global is not grown. A new object is the symbol size, clamped
 * to the load segment that contains base. */
static void filc_publish_data(void *base, size_t sz, unsigned flags)
{
    filc_object *o;
    filc_lock();
    o = filc_lookup(base);
    filc_unlock();
    if (o && (o->flags & FILC_FREE)) {
        filc_ret_cap = NULL;
        return;
    }
    if (!o)
        filc_cover(base, sz, flags);
    filc_ret_cap = base;
}

/* Classify the address dlsym returned. Do not substitute an ent_*
 * wrapper: the caller may store the bits as an integer relocation. */
static void filc_publish_sym(void *p)
{
    Dl_info info;
    const ElfW(Sym) *sym = NULL;
    struct filc_seg seg;
    unsigned type;
    void *base;
    size_t sz;
    unsigned flags;

    if (!p) {
        filc_ret_cap = NULL;
        return;
    }
    memset(&info, 0, sizeof info);
    if (dladdr1(p, &info, (void **)&sym, RTLD_DL_SYMENT) && sym) {
        type = ELF64_ST_TYPE(sym->st_info);
        if (type == STT_FUNC || type == STT_GNU_IFUNC) {
            filc_publish_func(p);
            return;
        }
        if (type == STT_OBJECT && sym->st_size > 0) {
            base = info.dli_saddr ? info.dli_saddr : p;
            sz = (size_t)sym->st_size;
            memset(&seg, 0, sizeof seg);
            seg.addr = (uintptr_t)base;
            dl_iterate_phdr(filc_seg_cb, &seg);
            if (!seg.found || (uintptr_t)base >= seg.hi)
                sz = 0;
            else if (sz > seg.hi - (uintptr_t)base)
                sz = (size_t)(seg.hi - (uintptr_t)base);
            flags = (sz && seg.found && !seg.writable) ? FILC_READONLY : 0;
            filc_publish_data(base, sz, flags);
            return;
        }
        /* TLS, a zero-size object, or any other named type. Do not
         * turn it into code just because the bytes are executable. */
        base = info.dli_saddr ? info.dli_saddr : p;
        filc_publish_data(base, 0, 0);
        return;
    }
    /* Resolved IFUNC often has no symbol at the implementation.
     * Executable, non-writable bytes are still code. Anything else
     * (TLS, unknown data) is fail-closed: size 0, not callable. */
    memset(&seg, 0, sizeof seg);
    seg.addr = (uintptr_t)p;
    dl_iterate_phdr(filc_seg_cb, &seg);
    if (seg.found && seg.exec && !seg.writable)
        filc_publish_func(p);
    else
        filc_publish_data(p, 0, 0);
}

void *filc_dlopen(void *lower, const char *file, int flags)
{
    void *h;
    filc_object *o;
    if (file)
        filc_strlen(lower, file);
    h = dlopen(file, flags);
    if (!h) {
        filc_ret_cap = NULL;
        return NULL;
    }
    filc_cover(h, 0, FILC_DL);
    filc_lock();
    o = filc_lookup(h);
    if (o && o->size == 0 && !(o->flags & (FILC_FREE | FILC_HEAP | FILC_FUNC)))
        o->flags |= FILC_DL;
    if (!o || !(o->flags & FILC_DL)) {
        filc_unlock();
        filc_panic("bad capability");
    }
    filc_unlock();
    filc_ret_cap = h;
    return h;
}

void *filc_dlsym(void *hl, void *handle, void *nl, const char *name)
{
    void *p;
    if (handle != RTLD_DEFAULT && handle != RTLD_NEXT)
        filc_dl_handle(hl, handle);
    filc_strlen(nl, name);
    p = dlsym(handle, name);
    filc_publish_sym(p);
    return p;
}

int filc_dlclose(void *lower, void *handle)
{
    struct link_map *map = NULL;
    uintptr_t base = 0;
    int have_base = 0;
    int r;
    filc_dl_handle(lower, handle);
    if (dlinfo(handle, RTLD_DI_LINKMAP, &map) == 0 && map) {
        base = (uintptr_t)map->l_addr;
        have_base = 1;
    }
    r = dlclose(handle);
    if (r != 0 || !have_base)
        return r;
    if (!filc_mapping_present(base))
        filc_uncover(handle);
    return r;
}

char *filc_dlerror(void)
{
    char *s;
    size_t n;
    s = dlerror();
    if (!s) {
        filc_ret_cap = NULL;
        return NULL;
    }
    n = strlen(s);
    if (n == (size_t)-1)
        filc_panic("out of bounds");
    filc_cover(s, n + 1, FILC_READONLY);
    filc_ret_cap = s;
    return s;
}

/* Cover strlen+1 of the libc string. lower == that pointer, and the
 * bytes stay writable. A missing variable returns NULL. */
static char *filc_env_get(void *lower, const char *name, int secure)
{
    char *r;
    size_t n;
    filc_strlen(lower, name);
    r = secure ? secure_getenv(name) : getenv(name);
    if (!r)
        return NULL;
    n = strlen(r);
    if (n == (size_t)-1)
        filc_panic("out of bounds");
    filc_cover(r, n + 1, 0);
    return r;
}

/* libc does not publish a return capability. The caller of an
 * unintercepted pointer function still does filc_get_ret, so a stale
 * lower (whatever the previous call returned) is checked against the
 * errno address and panics "out of bounds". Cover the int and publish
 * it. The address is per-thread and stable; cover is idempotent. */
int *__errno_location(void)
{
    static int *(*real)(void);
    int *p;

    if (!real) {
        real = (int *(*)(void))dlsym(RTLD_NEXT, "__errno_location");
        if (!real)
            abort();
    }
    p = real();
    if (filc_in_errno)
        return p;
    filc_in_errno = 1;
    if (p)
        filc_cover(p, sizeof(int), 0);
    filc_ret_cap = p;
    filc_in_errno = 0;
    return p;
}

char *filc_getenv(void *lower, const char *name)
{
    return filc_env_get(lower, name, 0);
}

char *filc_secure_getenv(void *lower, const char *name)
{
    return filc_env_get(lower, name, 1);
}

int filc_setenv(void *nl, const char *name, void *vl, const char *value,
    int overwrite)
{
    filc_strlen(nl, name);
    filc_strlen(vl, value);
    return setenv(name, value, overwrite);
}

int filc_unsetenv(void *lower, const char *name)
{
    filc_strlen(lower, name);
    return unsetenv(name);
}

int filc_putenv(void *lower, char *string)
{
    filc_strlen(lower, string);
    return putenv(string);
}

/* has_mode is 0 when the caller omitted open's optional mode. */
int filc_open(void *lower, const char *path, int flags, unsigned mode,
    int has_mode)
{
    filc_strlen(lower, path);
    if (has_mode)
        return open(path, flags, (mode_t)mode);
    return open(path, flags);
}

/* FILE* is opaque. The caller keeps using the libc pointer bits. */
FILE *filc_fopen(void *pl, const char *path, void *ml, const char *mode)
{
    filc_strlen(pl, path);
    filc_strlen(ml, mode);
    return fopen(path, mode);
}

FILE *filc_freopen(void *pl, const char *path, void *ml, const char *mode,
    FILE *stream)
{
    /* POSIX allows a null path: reopen the stream's current file. */
    if (path)
        filc_strlen(pl, path);
    filc_strlen(ml, mode);
    return freopen(path, mode, stream);
}

FILE *filc_fdopen(int fd, void *ml, const char *mode)
{
    filc_strlen(ml, mode);
    return fdopen(fd, mode);
}

int filc_puts(void *lower, const char *s)
{
    filc_strlen(lower, s);
    return puts(s);
}

int filc_fputs(void *lower, const char *s, FILE *stream)
{
    filc_strlen(lower, s);
    return fputs(s, stream);
}

/* fclose, fflush, fputc, putchar, close, and lseek carry no buffer.
 * FILE* stays the libc pointer bits. fflush(NULL) flushes every stream. */
int filc_fclose(FILE *stream)
{
    return fclose(stream);
}

int filc_fflush(FILE *stream)
{
    return fflush(stream);
}

int filc_fputc(int c, FILE *stream)
{
    return fputc(c, stream);
}

int filc_putchar(int c)
{
    return putchar(c);
}

int filc_close(int fd)
{
    return close(fd);
}

off_t filc_lseek(int fd, off_t offset, int whence)
{
    return lseek(fd, offset, whence);
}

void filc_perror(void *lower, const char *s)
{
    if (s)
        filc_strlen(lower, s);
    perror(s);
}

/* Bytes from ptr to the end of the object. One-past returns 0.
 * Read-only and out-of-range panic. The lock is not held on return. */
static size_t filc_writable_tail(void *lower, void *ptr)
{
    filc_object *o;
    uintptr_t p, lo, hi;
    size_t avail;
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(lower, "bad capability");
    if (o->flags & FILC_READONLY) {
        filc_unlock();
        filc_panic("write to read-only");
    }
    p = (uintptr_t)ptr;
    lo = (uintptr_t)o->payload;
    hi = lo + o->size;
    if (p < lo || p > hi) {
        filc_unlock();
        filc_panic("out of bounds");
    }
    avail = (size_t)(hi - p);
    filc_unlock();
    return avail;
}

/* Direct printf records each argument, then filc_fmt_end walks the
 * format. vprintf and an indirect printf walk the same conversions.
 * A pointer argument takes one capability. An int, a double, or a
 * long double only advances the va_list, so the next %s stays aligned.
 * A positional %N$ stops the walk after the format has been strlen'd.
 * A dynamic .* precision requires a terminator instead of a byte count. */
#define FILC_FMT_MAX 64
static __thread int filc_fmt_n;
static __thread const char *filc_fmt_s;
static __thread char filc_fmt_isptr[FILC_FMT_MAX];
static __thread void *filc_fmt_cap[FILC_FMT_MAX];
static __thread void *filc_fmt_arg[FILC_FMT_MAX];

void filc_fmt_begin(void *lower, const char *fmt)
{
    filc_strlen(lower, fmt);
    filc_fmt_n = 0;
    filc_fmt_s = fmt;
}

void filc_fmt_ptr(void *lower, void *p)
{
    if (filc_fmt_n >= FILC_FMT_MAX)
        filc_panic("out of bounds");
    filc_fmt_isptr[filc_fmt_n] = 1;
    filc_fmt_cap[filc_fmt_n] = lower;
    filc_fmt_arg[filc_fmt_n] = p;
    filc_fmt_n++;
}

void filc_fmt_skip(void)
{
    if (filc_fmt_n >= FILC_FMT_MAX)
        filc_panic("out of bounds");
    filc_fmt_isptr[filc_fmt_n] = 0;
    filc_fmt_cap[filc_fmt_n] = NULL;
    filc_fmt_arg[filc_fmt_n] = NULL;
    filc_fmt_n++;
}

static int filc_fmt_take(int *pi)
{
    int i;
    i = *pi;
    if (i < 0 || i >= filc_fmt_n)
        filc_panic("out of bounds");
    *pi = i + 1;
    return i;
}

/* 1 if s is at N$ or *N$ (N starts at 1). That form is positional. */
static int filc_fmt_pos(const char *s)
{
    if (*s == '*')
        s++;
    if (*s < '1' || *s > '9')
        return 0;
    while (*s >= '0' && *s <= '9')
        s++;
    return *s == '$';
}

/* cls 0 is a general register, 1 a double, 2 a long double.
 * want_ptr means this argument is a pointer. Mode 0 then requires a
 * recorded pointer. Modes 1 and 2 va_arg it and take one capability.
 * __va_arg is instrumented, so each va_arg gets its own one-entry
 * frame. That frame is popped before the capability is read, or the
 * caller's caps would be hidden. */
static void *filc_fmt_advance(int cls, va_list *ap, void *base)
{
    void *p;
    filc_call_push();
    filc_call_set(0, base);
    if (cls == 1) {
        (void)va_arg(*ap, double);
        p = NULL;
    } else if (cls == 2) {
        (void)va_arg(*ap, long double);
        p = NULL;
    } else {
        p = va_arg(*ap, void *);
    }
    filc_call_pop();
    return p;
}

static void filc_fmt_consume(int mode, va_list *ap, int *ai, int *cap_i,
    void *save, int cls, int want_ptr, void **ocap, void **op)
{
    void *p;
    int i;
    if (mode == 0) {
        i = filc_fmt_take(ai);
        if (!want_ptr)
            return;
        if (!filc_fmt_isptr[i])
            filc_panic("out of bounds");
        if (ocap)
            *ocap = filc_fmt_cap[i];
        if (op)
            *op = filc_fmt_arg[i];
        return;
    }
    p = filc_fmt_advance(cls, ap, (void *)*ap);
    if (!want_ptr)
        return;
    if (op)
        *op = p;
    if (mode == 1) {
        if (ocap)
            *ocap = filc_call_get(*cap_i);
        (*cap_i)++;
    } else if (ocap) {
        *ocap = filc_take_save(save);
    }
}

/* mode 0: arguments recorded by filc_fmt_ptr. mode 1: this call's
 * frame (indirect printf). mode 2: the va_list save area bound at
 * va_start (vprintf and a wrapper that passes the va_list on). */
static void filc_fmt_walk(int mode, va_list *ap, int cap_i, void *save)
{
    const char *s;
    int ai;

    s = filc_fmt_s;
    ai = 0;
    if (!s)
        return;
    while (*s) {
        int star_prec, has_prec, len, cls, want;
        long prec;
        char spec;
        size_t sz;
        void *cap, *p;

        if (*s != '%') {
            s++;
            continue;
        }
        s++;
        if (*s == '%') {
            s++;
            continue;
        }
        while (*s == '-' || *s == '+' || *s == ' ' || *s == '#'
            || *s == '0' || *s == '\'' || *s == 'I')
            s++;
        /* Positional conversions are not walked. The format is in bounds. */
        if (filc_fmt_pos(s))
            return;
        if (*s == '*') {
            filc_fmt_consume(mode, ap, &ai, &cap_i, save, 0, 0, NULL, NULL);
            s++;
        } else {
            while (*s >= '0' && *s <= '9')
                s++;
        }
        star_prec = 0;
        has_prec = 0;
        prec = 0;
        if (*s == '.') {
            s++;
            if (filc_fmt_pos(s))
                return;
            if (*s == '*') {
                filc_fmt_consume(mode, ap, &ai, &cap_i, save, 0, 0, NULL, NULL);
                star_prec = 1;
                s++;
            } else {
                has_prec = 1;
                while (*s >= '0' && *s <= '9') {
                    if (prec <= 1000000000L)
                        prec = prec * 10 + (long)(*s - '0');
                    if (prec > 1000000000L)
                        has_prec = 0;
                    s++;
                }
            }
        }
        len = 0;
        if (*s == 'h') {
            s++;
            if (*s == 'h') {
                len = 1;
                s++;
            } else {
                len = 2;
            }
        } else if (*s == 'l') {
            s++;
            if (*s == 'l') {
                len = 4;
                s++;
            } else {
                len = 3;
            }
        } else if (*s == 'z' || *s == 't' || *s == 'j' || *s == 'q') {
            len = (*s == 'q') ? 4 : 5;
            s++;
        } else if (*s == 'L') {
            len = 6;
            s++;
        }
        spec = *s;
        if (spec == 0)
            filc_panic("out of bounds");
        s++;
        if (spec == 'm')
            continue;
        /* %ls is a wide string. Consume the pointer and do not walk bytes.
         * %p is a pointer too. Mode 0 still only requires a recorded
         * pointer for %s and %n, which is what direct printf did. */
        want = (spec == 'n') || (spec == 's' && len != 3);
        if (mode != 0 && (spec == 's' || spec == 'n' || spec == 'p'))
            want = 1;
        if (spec == 's' || spec == 'n' || spec == 'p') {
            cap = NULL;
            p = NULL;
            filc_fmt_consume(mode, ap, &ai, &cap_i, save, 0, want, &cap, &p);
            if (spec == 's' && len != 3) {
                if (star_prec || !has_prec)
                    filc_strlen(cap, (const char *)p);
                else
                    filc_check(cap, p, (size_t)prec, 0);
            } else if (spec == 'n') {
                sz = 4;
                if (len == 1)
                    sz = 1;
                else if (len == 2)
                    sz = 2;
                else if (len >= 3)
                    sz = 8;
                filc_check(cap, p, sz, 1);
            }
            continue;
        }
        cls = 0;
        if (spec == 'a' || spec == 'A' || spec == 'e' || spec == 'E'
            || spec == 'f' || spec == 'F' || spec == 'g' || spec == 'G')
            cls = (len == 6) ? 2 : 1;
        filc_fmt_consume(mode, ap, &ai, &cap_i, save, cls, 0, NULL, NULL);
    }
}

void filc_fmt_end(void)
{
    filc_fmt_walk(0, NULL, 0, NULL);
}

static void filc_fmt_va(int mode, va_list *ap, int cap_i, void *save)
{
    void *base;
    base = (void *)*ap;
    filc_cover(base, sizeof(va_list), 0);
    filc_fmt_walk(mode, ap, cap_i, save);
    filc_uncover(base);
}

/* Check each %s and %n hidden in ap. The format was recorded by
 * filc_fmt_begin. x86_64 keeps reg_save_area at offset 16. */
void filc_vprint(void *al, va_list ap)
{
    va_list aq;
    void *save;
    filc_check(al, ap, sizeof(va_list), 1);
#if defined(__x86_64__)
    save = *(void **)((char *)ap + 16);
#else
    (void)save;
    filc_panic("out of bounds");
#endif
    va_copy(aq, ap);
    filc_fmt_va(2, &aq, 0, save);
    va_end(aq);
}

/* Indirect printf. var_cap is the first variadic pointer. An int
 * argument is not a capability, so it does not advance that index. */
static void filc_fmt_indirect(int fmt_cap, const char *fmt, va_list ap,
    int var_cap)
{
    va_list aq;
    filc_fmt_begin(filc_call_get(fmt_cap), fmt);
    va_copy(aq, ap);
    filc_fmt_va(1, &aq, var_cap, NULL);
    va_end(aq);
}

/* scanf integer size. len matches the printf walker: 0 int, 1 hh, 2 h,
 * 3 l, 4 ll, 5 z/t/j, 6 L. */
static size_t filc_scan_int_size(int len)
{
    if (len == 1)
        return 1;
    if (len == 2)
        return 2;
    if (len == 4)
        return 8;
    if (len == 3 || len == 5)
        return sizeof(long);
    if (len == 6)
        filc_panic("unchecked sscanf");
    return 4;
}

/* mode 0: destinations recorded by filc_fmt_ptr.
 * mode 1: this call's frame (indirect sscanf/fscanf/scanf).
 * mode 2: the va_list save area bound at va_start. */
static void *filc_scan_arg(va_list *ap, void *base);
static void filc_scan_check_one(int *ai, int mode, va_list *ap, int *cap_i,
    void *save, size_t sz)
{
    void *p, *cap;
    int i;
    if (mode == 1) {
        p = filc_scan_arg(ap, (void *)*ap);
        cap = filc_call_get(*cap_i);
        (*cap_i)++;
    } else if (mode == 2) {
        p = filc_scan_arg(ap, (void *)*ap);
        cap = filc_take_save(save);
    } else {
        i = filc_fmt_take(ai);
        if (!filc_fmt_isptr[i])
            filc_panic("out of bounds");
        p = filc_fmt_arg[i];
        cap = filc_fmt_cap[i];
    }
    filc_check(cap, p, sz, 1);
}

/* Direct sscanf records destinations with the printf helpers, then
 * calls this. An indirect sscanf passes mode 1 so the pointers come
 * from the va_list and the caps come from the call frame. vsscanf
 * passes mode 2 and the register save area. */
static void filc_scan_walk(int mode, va_list *ap, int cap_i, void *save)
{
    const char *s;
    int ai;

    s = filc_fmt_s;
    ai = 0;
    if (!s)
        return;
    while (*s) {
        int suppress, len;
        long width;
        char spec;
        size_t sz;

        if (*s != '%') {
            s++;
            continue;
        }
        s++;
        if (*s == '%') {
            s++;
            continue;
        }
        suppress = 0;
        if (*s == '*') {
            suppress = 1;
            s++;
        }
        if (filc_fmt_pos(s) || *s == 'm')
            filc_panic("unchecked sscanf");
        width = -1;
        if (*s >= '0' && *s <= '9') {
            width = 0;
            while (*s >= '0' && *s <= '9') {
                if (width > 100000000L)
                    filc_panic("out of bounds");
                width = width * 10 + (long)(*s - '0');
                s++;
            }
        }
        len = 0;
        if (*s == 'h') {
            s++;
            if (*s == 'h') {
                len = 1;
                s++;
            } else {
                len = 2;
            }
        } else if (*s == 'l') {
            s++;
            if (*s == 'l') {
                len = 4;
                s++;
            } else {
                len = 3;
            }
        } else if (*s == 'z' || *s == 't' || *s == 'j' || *s == 'q') {
            len = (*s == 'q') ? 4 : 5;
            s++;
        } else if (*s == 'L') {
            len = 6;
            s++;
        }
        spec = *s;
        if (spec == 0)
            filc_panic("out of bounds");
        s++;
        sz = 0;
        switch (spec) {
        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
        case 'n':
            sz = filc_scan_int_size(len);
            break;
        case 'c':
            if (len == 3)
                filc_panic("unchecked sscanf");
            sz = (width > 0) ? (size_t)width : 1;
            break;
        case 's':
            /* A missing width would let libc write until whitespace. */
            if (len == 3 || width < 0)
                filc_panic("unchecked sscanf");
            sz = (size_t)width + 1;
            break;
        case 'a':
        case 'e':
        case 'f':
        case 'g':
        case 'A':
        case 'E':
        case 'F':
        case 'G':
            if (len == 6)
                sz = sizeof(long double);
            else if (len == 3 || len == 4)
                sz = sizeof(double);
            else if (len == 0)
                sz = sizeof(float);
            else
                filc_panic("unchecked sscanf");
            break;
        default:
            filc_panic("unchecked sscanf");
        }
        if (suppress)
            continue;
        filc_scan_check_one(&ai, mode, ap, &cap_i, save, sz);
    }
}

void filc_scan_end(void)
{
    filc_scan_walk(0, NULL, 0, NULL);
}

/* __va_arg in libtcc1-filc.a is instrumented. It reads call cap 0 as
 * the va_list. This file is not compiled with -filc, so the call would
 * otherwise see the caller's frame. Cover the list the walk advances
 * (a copy; libc still uses the caller's list) and drop it after.
 * The frame exists only for the va_arg, so mode 1 still reads
 * destination caps from the call that entered the runtime. */
static void *filc_scan_arg(va_list *ap, void *base)
{
    void *p;
    filc_call_push();
    filc_call_set(0, base);
    p = va_arg(*ap, void *);
    filc_call_pop();
    return p;
}

static void filc_scan_va(int mode, va_list *ap, int cap_i, void *save)
{
    void *base;
    base = (void *)*ap;
    filc_cover(base, sizeof(va_list), 0);
    filc_scan_walk(mode, ap, cap_i, save);
    filc_uncover(base);
}

/* Check each destination hidden in ap. The format was recorded by
 * filc_fmt_begin. x86_64 keeps reg_save_area at offset 16. */
void filc_vscan(void *al, va_list ap)
{
    va_list aq;
    void *save;
    filc_check(al, ap, sizeof(va_list), 1);
#if defined(__x86_64__)
    save = *(void **)((char *)ap + 16);
#else
    (void)save;
    filc_panic("unchecked sscanf");
#endif
    va_copy(aq, ap);
    filc_scan_va(2, &aq, 0, save);
    va_end(aq);
}

/* The printed text plus its NUL must fit. vsnprintf never writes past
 * the object; a truncated result is still a panic. */
int filc_sprintf(void *dl, char *dst, const char *fmt, ...)
{
    size_t avail;
    va_list ap;
    int n;
    avail = filc_writable_tail(dl, dst);
    if (avail == 0)
        filc_panic("out of bounds");
    va_start(ap, fmt);
    n = vsnprintf(dst, avail, fmt, ap);
    va_end(ap);
    if (n < 0)
        return n;
    if ((size_t)n >= avail)
        filc_panic("out of bounds");
    return n;
}

int filc_vsprintf(void *dl, char *dst, const char *fmt, va_list ap)
{
    size_t avail;
    int n;
    avail = filc_writable_tail(dl, dst);
    if (avail == 0)
        filc_panic("out of bounds");
    n = vsnprintf(dst, avail, fmt, ap);
    if (n < 0)
        return n;
    if ((size_t)n >= avail)
        filc_panic("out of bounds");
    return n;
}

/* n is the caller's bound, not the printed length. Truncation is fine.
 * snprintf(NULL, 0, ...) is the sizing call and is not a checked write. */
int filc_snprintf(void *dl, char *dst, size_t n, const char *fmt, ...)
{
    va_list ap;
    int r;
    if (!(n == 0 && !dst))
        filc_check(dl, dst, n, 1);
    va_start(ap, fmt);
    r = vsnprintf(dst, n, fmt, ap);
    va_end(ap);
    return r;
}

int filc_vsnprintf(void *dl, char *dst, size_t n, const char *fmt, va_list ap)
{
    if (!(n == 0 && !dst))
        filc_check(dl, dst, n, 1);
    return vsnprintf(dst, n, fmt, ap);
}

/* Cover the mapping. MAP_FAILED and NULL are not objects. The mapping
 * is not tracked: it outlives the frame. No PROT_WRITE is read-only,
 * including PROT_NONE, whose reads the kernel may still fault. */
void *filc_mmap(void *addr, size_t length, int prot, int flags, int fd,
    off_t offset)
{
    void *p;
    unsigned fl;
    p = mmap(addr, length, prot, flags, fd, offset);
    if (p == MAP_FAILED || !p)
        return p;
    fl = (prot & PROT_WRITE) ? 0 : FILC_READONLY;
    filc_cover(p, length, fl);
    return p;
}

/* The address and length must be the whole object. A prefix is rejected
 * rather than splitting it. A failed syscall leaves the object in place. */
int filc_munmap(void *lower, void *addr, size_t length)
{
    filc_object *o;
    int r;
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(lower, "bad capability");
    if (o->payload != addr || o->size != length) {
        filc_unlock();
        filc_panic("out of bounds");
    }
    filc_unlock();
    r = munmap(addr, length);
    if (r == 0)
        filc_uncover(lower);
    return r;
}

/* Same whole-object rule. Dropping PROT_WRITE marks the object
 * read-only. Adding PROT_WRITE does not clear that bit. */
int filc_mprotect(void *lower, void *addr, size_t length, int prot)
{
    filc_object *o;
    int r;
    if (!filc_inits_done)
        filc_apply_inits();
    filc_lock();
    o = filc_need(lower, "bad capability");
    if (o->payload != addr || o->size != length) {
        filc_unlock();
        filc_panic("out of bounds");
    }
    filc_unlock();
    r = mprotect(addr, length, prot);
    if (r == 0 && !(prot & PROT_WRITE)) {
        filc_lock();
        o = filc_lookup(lower);
        if (o && !(o->flags & FILC_FREE))
            o->flags |= FILC_READONLY;
        filc_unlock();
    }
    return r;
}

/* qsort must not be the libc function: a byte swap leaves aux in the
 * old slots. This heapsort moves those slots with filc_memmove. The
 * GC in this file still calls libc qsort by name. */
#define FILC_SORT_MAX 8
typedef int (*filc_cmp_fn)(const void *, const void *);
struct filc_sort_frame {
    void *base_lower;
    void *key_lower;
    filc_cmp_fn cmp;
};
static __thread struct filc_sort_frame filc_sort_stk[FILC_SORT_MAX];
static __thread int filc_sort_depth;

/* Which object contains p. Caller holds the lock. A miss is a null
 * cap, not a panic: filc_need would abort without unlocking. */
static void *filc_ptr_in(void *lower, uintptr_t a)
{
    filc_object *o;
    uintptr_t lo, hi;
    if (!lower)
        return NULL;
    o = filc_lookup(lower);
    if (!o || (o->flags & FILC_FREE) || !o->size)
        return NULL;
    lo = (uintptr_t)o->payload;
    hi = lo + o->size;
    if (a < lo || a >= hi)
        return NULL;
    return lower;
}

static void *filc_sort_cap(const void *p)
{
    struct filc_sort_frame *f;
    void *cap;
    if (filc_sort_depth <= 0)
        return NULL;
    f = &filc_sort_stk[filc_sort_depth - 1];
    filc_lock();
    cap = filc_ptr_in(f->base_lower, (uintptr_t)p);
    if (!cap)
        cap = filc_ptr_in(f->key_lower, (uintptr_t)p);
    filc_unlock();
    return cap;
}

static int filc_sort_thunk(const void *a, const void *b)
{
    struct filc_sort_frame *f;
    void *la, *lb;
    int r;
    f = &filc_sort_stk[filc_sort_depth - 1];
    la = filc_sort_cap(a);
    lb = filc_sort_cap(b);
    filc_call_push();
    filc_call_set(0, la);
    filc_call_set(1, lb);
    r = f->cmp(a, b);
    filc_call_pop();
    return r;
}

static void filc_sort_enter(void *base_lower, void *key_lower, filc_cmp_fn cmp)
{
    struct filc_sort_frame *f;
    if (filc_sort_depth >= FILC_SORT_MAX)
        filc_panic("too many qsort");
    f = &filc_sort_stk[filc_sort_depth];
    f->base_lower = base_lower;
    f->key_lower = key_lower;
    f->cmp = cmp;
    filc_sort_depth++;
}

static void filc_sort_leave(void)
{
    if (filc_sort_depth > 0)
        filc_sort_depth--;
}

static void filc_elem_swap(void *bl, char *a, char *b, size_t size,
    void *tl, void *tmp)
{
    if (a == b)
        return;
    filc_memmove(tl, tmp, bl, a, size);
    filc_memmove(bl, a, bl, b, size);
    filc_memmove(bl, b, tl, tmp, size);
}

/* Max-heap sift. end is inclusive. end == 0 is a single element. */
static void filc_sift(void *bl, char *base, size_t root, size_t end,
    size_t size, void *tl, void *tmp)
{
    while (end >= 1 && root <= (end - 1) / 2) {
        size_t child = root * 2 + 1;
        size_t swap = root;
        if (filc_sort_thunk(base + swap * size, base + child * size) < 0)
            swap = child;
        if (child + 1 <= end &&
            filc_sort_thunk(base + swap * size, base + (child + 1) * size) < 0)
            swap = child + 1;
        if (swap == root)
            return;
        filc_elem_swap(bl, base + root * size, base + swap * size, size, tl, tmp);
        root = swap;
    }
}

static void filc_heapsort(void *bl, char *base, size_t n, size_t size,
    void *tl, void *tmp)
{
    size_t start, end;
    if (n < 2)
        return;
    start = (n - 2) / 2;
    for (;;) {
        filc_sift(bl, base, start, n - 1, size, tl, tmp);
        if (start == 0)
            break;
        start--;
    }
    end = n - 1;
    while (end > 0) {
        filc_elem_swap(bl, base, base + end * size, size, tl, tmp);
        end--;
        filc_sift(bl, base, 0, end, size, tl, tmp);
    }
}

void filc_qsort(void *bl, void *base, size_t nmemb, size_t size,
    void *cl, filc_cmp_fn compar)
{
    void *tmp;
    size_t nbytes;
    if (!nmemb || !size)
        return;
    nbytes = filc_io_bytes(size, nmemb);
    filc_check(bl, base, nbytes, 1);
    filc_check_func(cl, (void *)compar);
    tmp = malloc(size);
    if (!tmp)
        filc_panic("out of memory");
    filc_cover(tmp, size, 0);
    filc_sort_enter(bl, NULL, compar);
    filc_heapsort(bl, (char *)base, nmemb, size, tmp, tmp);
    filc_sort_leave();
    filc_uncover(tmp);
    free(tmp);
}

/* A hit is an interior pointer. The capability stays the array. */
void *filc_bsearch(void *kl, const void *key, void *bl, const void *base,
    size_t nmemb, size_t size, void *cl, filc_cmp_fn compar)
{
    void *r;
    size_t nbytes;
    if (!nmemb || !size) {
        filc_ret_cap = NULL;
        return NULL;
    }
    nbytes = filc_io_bytes(size, nmemb);
    filc_check(bl, (void *)base, nbytes, 0);
    filc_check(kl, (void *)key, size, 0);
    filc_check_func(cl, (void *)compar);
    filc_sort_enter(bl, kl, compar);
    r = bsearch(key, base, nmemb, size, filc_sort_thunk);
    filc_sort_leave();
    filc_ret_cap = r ? bl : NULL;
    return r;
}

int filc_strcasecmp(void *al, const char *a, void *bl, const char *b)
{
    filc_strlen(al, a);
    filc_strlen(bl, b);
    return strcasecmp(a, b);
}

int filc_strncasecmp(void *al, const char *a, void *bl, const char *b, size_t n)
{
    filc_strnlen(al, a, n);
    filc_strnlen(bl, b, n);
    return strncasecmp(a, b, n);
}

/* A hit may be interior. The capability stays the haystack. */
char *filc_strcasestr(void *hl, const char *h, void *nl, const char *n)
{
    char *r;
    filc_strlen(hl, h);
    filc_strlen(nl, n);
    r = strcasestr(h, n);
    filc_ret_cap = r ? hl : NULL;
    return r;
}

/* An empty needle matches at the start and is not read. */
void *filc_memmem(void *hl, const void *hay, size_t hs, void *nl,
    const void *needle, size_t ns)
{
    void *r;
    filc_check(hl, (void *)hay, hs, 0);
    if (!ns) {
        filc_ret_cap = hl;
        return (void *)hay;
    }
    filc_check(nl, (void *)needle, ns, 0);
    r = memmem(hay, hs, needle, ns);
    filc_ret_cap = r ? hl : NULL;
    return r;
}

int filc_stat(void *pl, const char *path, void *bl, struct stat *buf)
{
    filc_strlen(pl, path);
    filc_check(bl, buf, sizeof(*buf), 1);
    return stat(path, buf);
}

int filc_lstat(void *pl, const char *path, void *bl, struct stat *buf)
{
    filc_strlen(pl, path);
    filc_check(bl, buf, sizeof(*buf), 1);
    return lstat(path, buf);
}

int filc_fstat(void *bl, int fd, struct stat *buf)
{
    filc_check(bl, buf, sizeof(*buf), 1);
    return fstat(fd, buf);
}

/* Same-signature entries for indirect calls. The compiler already
 * pushed a capability frame; index 0 is the first pointer argument. */
static void *ent_malloc(size_t n)
{
    void *p = filc_malloc(n);
    filc_set_ret(p);
    return p;
}

static void *ent_calloc(size_t a, size_t b)
{
    void *p = filc_calloc(a, b);
    filc_set_ret(p);
    return p;
}

static void *ent_realloc(void *p, size_t n)
{
    void *np = filc_realloc(filc_call_get(0), p, n);
    filc_set_ret(np);
    return np;
}

static void ent_free(void *p)
{
    filc_free(filc_call_get(0), p);
}

static void *ent_memcpy(void *d, const void *s, size_t n)
{
    return filc_memcpy(filc_call_get(0), d, filc_call_get(1), (void *)s, n);
}

static void *ent_memmove(void *d, const void *s, size_t n)
{
    return filc_memmove(filc_call_get(0), d, filc_call_get(1), (void *)s, n);
}

static void *ent_memset(void *d, int c, size_t n)
{
    return filc_memset(filc_call_get(0), d, c, n);
}

static size_t ent_strlen(const char *s)
{
    return filc_strlen(filc_call_get(0), s);
}

static int ent_strcmp(const char *a, const char *b)
{
    return filc_strcmp(filc_call_get(0), a, filc_call_get(1), b);
}

static int ent_strncmp(const char *a, const char *b, size_t n)
{
    return filc_strncmp(filc_call_get(0), a, filc_call_get(1), b, n);
}

static char *ent_strchr(const char *s, int c)
{
    return filc_strchr(filc_call_get(0), s, c);
}

static char *ent_strrchr(const char *s, int c)
{
    return filc_strrchr(filc_call_get(0), s, c);
}

static char *ent_strstr(const char *h, const char *n)
{
    return filc_strstr(filc_call_get(0), h, filc_call_get(1), n);
}

static void *ent_memchr(const void *s, int c, size_t n)
{
    return filc_memchr(filc_call_get(0), s, c, n);
}

static char *ent_strdup(const char *s)
{
    char *d = filc_strdup(filc_call_get(0), s);
    filc_set_ret(d);
    return d;
}

static char *ent_strcpy(char *d, const char *s)
{
    return filc_strcpy(filc_call_get(0), d, filc_call_get(1), s);
}

static char *ent_strncpy(char *d, const char *s, size_t n)
{
    return filc_strncpy(filc_call_get(0), d, filc_call_get(1), s, n);
}

static char *ent_strcat(char *d, const char *s)
{
    return filc_strcat(filc_call_get(0), d, filc_call_get(1), s);
}

static char *ent_strncat(char *d, const char *s, size_t n)
{
    return filc_strncat(filc_call_get(0), d, filc_call_get(1), s, n);
}

static size_t ent_strnlen(const char *s, size_t n)
{
    return filc_strnlen(filc_call_get(0), s, n);
}

static int ent_memcmp(const void *a, const void *b, size_t n)
{
    return filc_memcmp(filc_call_get(0), a, filc_call_get(1), b, n);
}

static char *ent_strndup(const char *s, size_t n)
{
    char *d = filc_strndup(filc_call_get(0), s, n);
    filc_set_ret(d);
    return d;
}

static char *ent_stpcpy(char *d, const char *s)
{
    return filc_stpcpy(filc_call_get(0), d, filc_call_get(1), s);
}

static char *ent_stpncpy(char *d, const char *s, size_t n)
{
    return filc_stpncpy(filc_call_get(0), d, filc_call_get(1), s, n);
}

static size_t ent_strspn(const char *s, const char *acc)
{
    return filc_strspn(filc_call_get(0), s, filc_call_get(1), acc);
}

static size_t ent_strcspn(const char *s, const char *rej)
{
    return filc_strcspn(filc_call_get(0), s, filc_call_get(1), rej);
}

static char *ent_strpbrk(const char *s, const char *acc)
{
    return filc_strpbrk(filc_call_get(0), s, filc_call_get(1), acc);
}

static void *ent_memccpy(void *d, const void *s, int c, size_t n)
{
    return filc_memccpy(filc_call_get(0), d, filc_call_get(1), s, c, n);
}

static void ent_explicit_bzero(void *d, size_t n)
{
    filc_explicit_bzero(filc_call_get(0), d, n);
}

static void ent_bzero(void *d, size_t n)
{
    filc_explicit_bzero(filc_call_get(0), d, n);
}

static char *ent_strtok(char *s, const char *delim)
{
    return filc_strtok(filc_call_get(0), s, filc_call_get(1), delim);
}

static char *ent_strtok_r(char *s, const char *delim, char **saveptr)
{
    return filc_strtok_r(filc_call_get(0), s, filc_call_get(1), delim,
        filc_call_get(2), saveptr);
}

static char *ent_strsep(char **stringp, const char *delim)
{
    return filc_strsep(filc_call_get(0), stringp, filc_call_get(1), delim);
}

static ssize_t ent_read(int fd, void *buf, size_t n)
{
    return filc_read(filc_call_get(0), fd, buf, n);
}

static ssize_t ent_write(int fd, const void *buf, size_t n)
{
    return filc_write(filc_call_get(0), fd, buf, n);
}

static ssize_t ent_pread(int fd, void *buf, size_t n, off_t off)
{
    return filc_pread(filc_call_get(0), fd, buf, n, off);
}

static ssize_t ent_pwrite(int fd, const void *buf, size_t n, off_t off)
{
    return filc_pwrite(filc_call_get(0), fd, buf, n, off);
}

static size_t ent_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return filc_fread(filc_call_get(0), ptr, size, nmemb, stream);
}

static size_t ent_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return filc_fwrite(filc_call_get(0), ptr, size, nmemb, stream);
}

static char *ent_fgets(char *s, int size, FILE *stream)
{
    return filc_fgets(filc_call_get(0), s, size, stream);
}

static char *ent_getenv(const char *name)
{
    char *r = filc_getenv(filc_call_get(0), name);
    filc_set_ret(r);
    return r;
}

static char *ent_secure_getenv(const char *name)
{
    char *r = filc_secure_getenv(filc_call_get(0), name);
    filc_set_ret(r);
    return r;
}

static int ent_setenv(const char *name, const char *value, int overwrite)
{
    return filc_setenv(filc_call_get(0), name, filc_call_get(1), value, overwrite);
}

static int ent_unsetenv(const char *name)
{
    return filc_unsetenv(filc_call_get(0), name);
}

static int ent_putenv(char *string)
{
    return filc_putenv(filc_call_get(0), string);
}

static int ent_open(const char *path, int flags, unsigned mode)
{
    return filc_open(filc_call_get(0), path, flags, mode, 1);
}

static FILE *ent_fopen(const char *path, const char *mode)
{
    FILE *fp = filc_fopen(filc_call_get(0), path, filc_call_get(1), mode);
    filc_set_ret(NULL);
    return fp;
}

static FILE *ent_freopen(const char *path, const char *mode, FILE *stream)
{
    FILE *fp = filc_freopen(filc_call_get(0), path, filc_call_get(1), mode, stream);
    filc_set_ret(NULL);
    return fp;
}

static FILE *ent_fdopen(int fd, const char *mode)
{
    FILE *fp = filc_fdopen(fd, filc_call_get(0), mode);
    filc_set_ret(NULL);
    return fp;
}

static int ent_puts(const char *s)
{
    return filc_puts(filc_call_get(0), s);
}

static int ent_fputs(const char *s, FILE *stream)
{
    return filc_fputs(filc_call_get(0), s, stream);
}

static int ent_fclose(FILE *stream)
{
    return filc_fclose(stream);
}

static int ent_fflush(FILE *stream)
{
    return filc_fflush(stream);
}

static int ent_close(int fd)
{
    return filc_close(fd);
}

static off_t ent_lseek(int fd, off_t offset, int whence)
{
    return filc_lseek(fd, offset, whence);
}

static void *ent_mmap(void *addr, size_t length, int prot, int flags, int fd,
    off_t offset)
{
    void *p = filc_mmap(addr, length, prot, flags, fd, offset);
    if (p == MAP_FAILED || !p)
        filc_set_ret(NULL);
    else
        filc_set_ret(p);
    return p;
}

static int ent_munmap(void *addr, size_t length)
{
    return filc_munmap(filc_call_get(0), addr, length);
}

static int ent_mprotect(void *addr, size_t length, int prot)
{
    return filc_mprotect(filc_call_get(0), addr, length, prot);
}

/* Variadic caps start after the named pointers. dprintf's fd and
 * snprintf's size are integers, so they are not capabilities.
 * Destination bounds still apply to *sprintf. */
static int ent_printf(const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    filc_fmt_indirect(0, fmt, ap, 1);
    r = vprintf(fmt, ap);
    va_end(ap);
    return r;
}

static int ent_fprintf(FILE *stream, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    filc_fmt_indirect(1, fmt, ap, 2);
    r = vfprintf(stream, fmt, ap);
    va_end(ap);
    return r;
}

static int ent_sprintf(char *dst, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    filc_fmt_indirect(1, fmt, ap, 2);
    r = filc_vsprintf(filc_call_get(0), dst, fmt, ap);
    va_end(ap);
    return r;
}

static int ent_snprintf(char *dst, size_t n, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    filc_fmt_indirect(1, fmt, ap, 2);
    r = filc_vsnprintf(filc_call_get(0), dst, n, fmt, ap);
    va_end(ap);
    return r;
}

static int ent_dprintf(int fd, const char *fmt, ...)
{
    va_list ap;
    int r;
    va_start(ap, fmt);
    filc_fmt_indirect(0, fmt, ap, 1);
    r = vdprintf(fd, fmt, ap);
    va_end(ap);
    return r;
}

static int ent_vprintf(const char *fmt, va_list ap)
{
    filc_fmt_begin(filc_call_get(0), fmt);
    filc_vprint(filc_call_get(1), ap);
    return vprintf(fmt, ap);
}

static int ent_vfprintf(FILE *stream, const char *fmt, va_list ap)
{
    filc_fmt_begin(filc_call_get(1), fmt);
    filc_vprint(filc_call_get(2), ap);
    return vfprintf(stream, fmt, ap);
}

static int ent_vsprintf(char *dst, const char *fmt, va_list ap)
{
    filc_fmt_begin(filc_call_get(1), fmt);
    filc_vprint(filc_call_get(2), ap);
    return filc_vsprintf(filc_call_get(0), dst, fmt, ap);
}

static int ent_vsnprintf(char *dst, size_t n, const char *fmt, va_list ap)
{
    filc_fmt_begin(filc_call_get(1), fmt);
    filc_vprint(filc_call_get(2), ap);
    return filc_vsnprintf(filc_call_get(0), dst, n, fmt, ap);
}

static int ent_fputc(int c, FILE *stream)
{
    return filc_fputc(c, stream);
}

static int ent_putchar(int c)
{
    return filc_putchar(c);
}

static int ent_putc(int c, FILE *stream)
{
    return filc_fputc(c, stream);
}

static void ent_perror(const char *s)
{
    if (s)
        filc_strlen(filc_call_get(0), s);
    perror(s);
}

static void ent_qsort(void *base, size_t nmemb, size_t size, filc_cmp_fn compar)
{
    filc_qsort(filc_call_get(0), base, nmemb, size, filc_call_get(1), compar);
}

static void *ent_bsearch(const void *key, const void *base, size_t nmemb,
    size_t size, filc_cmp_fn compar)
{
    void *r;
    r = filc_bsearch(filc_call_get(0), key, filc_call_get(1), base,
        nmemb, size, filc_call_get(2), compar);
    filc_set_ret(r ? filc_call_get(1) : NULL);
    return r;
}

static int ent_strcasecmp(const char *a, const char *b)
{
    return filc_strcasecmp(filc_call_get(0), a, filc_call_get(1), b);
}

static int ent_strncasecmp(const char *a, const char *b, size_t n)
{
    return filc_strncasecmp(filc_call_get(0), a, filc_call_get(1), b, n);
}

static char *ent_strcasestr(const char *h, const char *n)
{
    return filc_strcasestr(filc_call_get(0), h, filc_call_get(1), n);
}

static void *ent_memmem(const void *hay, size_t hs, const void *needle, size_t ns)
{
    return filc_memmem(filc_call_get(0), hay, hs, filc_call_get(1), needle, ns);
}

static int ent_stat(const char *path, struct stat *buf)
{
    return filc_stat(filc_call_get(0), path, filc_call_get(1), buf);
}

static int ent_lstat(const char *path, struct stat *buf)
{
    return filc_lstat(filc_call_get(0), path, filc_call_get(1), buf);
}

static int ent_fstat(int fd, struct stat *buf)
{
    return filc_fstat(filc_call_get(0), fd, buf);
}

/* libc walks until it sees a terminator. Require that terminator to
 * sit inside the source object, then publish the end pointer as an
 * interior pointer of that same object. */
static void filc_set_end(void *sl, char *e, void *el, char **end)
{
    if (!end)
        return;
    if (!e)
        filc_panic("bad capability");
    filc_check(sl, e, 1, 0);
    filc_store_ptr(el, end, sl, (uintptr_t)e);
}

long filc_strtol(void *sl, const char *s, void *el, char **end, int base)
{
    char *e = NULL;
    long v;
    filc_strlen(sl, s);
    v = strtol(s, end ? &e : NULL, base);
    filc_set_end(sl, e, el, end);
    return v;
}

long long filc_strtoll(void *sl, const char *s, void *el, char **end, int base)
{
    char *e = NULL;
    long long v;
    filc_strlen(sl, s);
    v = strtoll(s, end ? &e : NULL, base);
    filc_set_end(sl, e, el, end);
    return v;
}

unsigned long filc_strtoul(void *sl, const char *s, void *el, char **end,
    int base)
{
    char *e = NULL;
    unsigned long v;
    filc_strlen(sl, s);
    v = strtoul(s, end ? &e : NULL, base);
    filc_set_end(sl, e, el, end);
    return v;
}

unsigned long long filc_strtoull(void *sl, const char *s, void *el, char **end,
    int base)
{
    char *e = NULL;
    unsigned long long v;
    filc_strlen(sl, s);
    v = strtoull(s, end ? &e : NULL, base);
    filc_set_end(sl, e, el, end);
    return v;
}

float filc_strtof(void *sl, const char *s, void *el, char **end)
{
    char *e = NULL;
    float v;
    filc_strlen(sl, s);
    v = strtof(s, end ? &e : NULL);
    filc_set_end(sl, e, el, end);
    return v;
}

double filc_strtod(void *sl, const char *s, void *el, char **end)
{
    char *e = NULL;
    double v;
    filc_strlen(sl, s);
    v = strtod(s, end ? &e : NULL);
    filc_set_end(sl, e, el, end);
    return v;
}

long double filc_strtold(void *sl, const char *s, void *el, char **end)
{
    char *e = NULL;
    long double v;
    filc_strlen(sl, s);
    v = strtold(s, end ? &e : NULL);
    filc_set_end(sl, e, el, end);
    return v;
}

int filc_atoi(void *sl, const char *s)
{
    filc_strlen(sl, s);
    return atoi(s);
}

long filc_atol(void *sl, const char *s)
{
    filc_strlen(sl, s);
    return atol(s);
}

long long filc_atoll(void *sl, const char *s)
{
    filc_strlen(sl, s);
    return atoll(s);
}

double filc_atof(void *sl, const char *s)
{
    filc_strlen(sl, s);
    return atof(s);
}

static long ent_strtol(const char *s, char **end, int base)
{
    return filc_strtol(filc_call_get(0), s, filc_call_get(1), end, base);
}

static long long ent_strtoll(const char *s, char **end, int base)
{
    return filc_strtoll(filc_call_get(0), s, filc_call_get(1), end, base);
}

static unsigned long ent_strtoul(const char *s, char **end, int base)
{
    return filc_strtoul(filc_call_get(0), s, filc_call_get(1), end, base);
}

static unsigned long long ent_strtoull(const char *s, char **end, int base)
{
    return filc_strtoull(filc_call_get(0), s, filc_call_get(1), end, base);
}

static float ent_strtof(const char *s, char **end)
{
    return filc_strtof(filc_call_get(0), s, filc_call_get(1), end);
}

static double ent_strtod(const char *s, char **end)
{
    return filc_strtod(filc_call_get(0), s, filc_call_get(1), end);
}

static long double ent_strtold(const char *s, char **end)
{
    return filc_strtold(filc_call_get(0), s, filc_call_get(1), end);
}

static int ent_atoi(const char *s)
{
    return filc_atoi(filc_call_get(0), s);
}

static long ent_atol(const char *s)
{
    return filc_atol(filc_call_get(0), s);
}

static long long ent_atoll(const char *s)
{
    return filc_atoll(filc_call_get(0), s);
}

static double ent_atof(const char *s)
{
    return filc_atof(filc_call_get(0), s);
}

static char *ent_getcwd(char *buf, size_t size)
{
    return filc_getcwd(filc_call_get(0), buf, size);
}

static void *ent_dlopen(const char *file, int flags)
{
    return filc_dlopen(filc_call_get(0), file, flags);
}

static void *ent_dlsym(void *handle, const char *name)
{
    return filc_dlsym(filc_call_get(0), handle, filc_call_get(1), name);
}

static int ent_dlclose(void *handle)
{
    return filc_dlclose(filc_call_get(0), handle);
}

static char *ent_dlerror(void)
{
    return filc_dlerror();
}

static int ent_sscanf(const char *s, const char *fmt, ...)
{
    va_list ap, aq;
    int r;
    filc_strlen(filc_call_get(0), s);
    filc_fmt_begin(filc_call_get(1), fmt);
    va_start(ap, fmt);
    va_copy(aq, ap);
    filc_scan_va(1, &aq, 2, NULL);
    va_end(aq);
    r = vsscanf(s, fmt, ap);
    va_end(ap);
    return r;
}

static int ent_fscanf(FILE *fp, const char *fmt, ...)
{
    va_list ap, aq;
    int r;
    filc_fmt_begin(filc_call_get(1), fmt);
    va_start(ap, fmt);
    va_copy(aq, ap);
    filc_scan_va(1, &aq, 2, NULL);
    va_end(aq);
    r = vfscanf(fp, fmt, ap);
    va_end(ap);
    return r;
}

static int ent_scanf(const char *fmt, ...)
{
    va_list ap, aq;
    int r;
    filc_fmt_begin(filc_call_get(0), fmt);
    va_start(ap, fmt);
    va_copy(aq, ap);
    filc_scan_va(1, &aq, 1, NULL);
    va_end(aq);
    r = vscanf(fmt, ap);
    va_end(ap);
    return r;
}

static int ent_vsscanf(const char *s, const char *fmt, va_list ap)
{
    int r;
    filc_strlen(filc_call_get(0), s);
    filc_fmt_begin(filc_call_get(1), fmt);
    filc_vscan(filc_call_get(2), ap);
    r = vsscanf(s, fmt, ap);
    return r;
}

static int ent_vfscanf(FILE *fp, const char *fmt, va_list ap)
{
    int r;
    filc_fmt_begin(filc_call_get(1), fmt);
    filc_vscan(filc_call_get(2), ap);
    r = vfscanf(fp, fmt, ap);
    return r;
}

static int ent_vscanf(const char *fmt, va_list ap)
{
    int r;
    filc_fmt_begin(filc_call_get(0), fmt);
    filc_vscan(filc_call_get(1), ap);
    r = vscanf(fmt, ap);
    return r;
}

#define FILC_NENTS 102
static struct {
    const char *name;
    void *real;
    void *entry;
} filc_ents[FILC_NENTS];

void *filc_adapt(void *fn)
{
    int i;
    if (!fn)
        return fn;
    for (i = 0; i < FILC_NENTS; i++) {
        if (filc_ents[i].real && fn == filc_ents[i].real)
            return filc_ents[i].entry;
    }
    return fn;
}

static void filc_resolve_libc(void)
{
    int i;
    filc_ents[0].name = "malloc"; filc_ents[0].entry = (void *)ent_malloc;
    filc_ents[1].name = "calloc"; filc_ents[1].entry = (void *)ent_calloc;
    filc_ents[2].name = "realloc"; filc_ents[2].entry = (void *)ent_realloc;
    filc_ents[3].name = "free"; filc_ents[3].entry = (void *)ent_free;
    filc_ents[4].name = "memcpy"; filc_ents[4].entry = (void *)ent_memcpy;
    filc_ents[5].name = "memmove"; filc_ents[5].entry = (void *)ent_memmove;
    filc_ents[6].name = "memset"; filc_ents[6].entry = (void *)ent_memset;
    filc_ents[7].name = "strlen"; filc_ents[7].entry = (void *)ent_strlen;
    filc_ents[8].name = "strcmp"; filc_ents[8].entry = (void *)ent_strcmp;
    filc_ents[9].name = "strncmp"; filc_ents[9].entry = (void *)ent_strncmp;
    filc_ents[10].name = "strchr"; filc_ents[10].entry = (void *)ent_strchr;
    filc_ents[11].name = "strrchr"; filc_ents[11].entry = (void *)ent_strrchr;
    filc_ents[12].name = "strstr"; filc_ents[12].entry = (void *)ent_strstr;
    filc_ents[13].name = "memchr"; filc_ents[13].entry = (void *)ent_memchr;
    filc_ents[14].name = "strdup"; filc_ents[14].entry = (void *)ent_strdup;
    filc_ents[15].name = "strcpy"; filc_ents[15].entry = (void *)ent_strcpy;
    filc_ents[16].name = "strncpy"; filc_ents[16].entry = (void *)ent_strncpy;
    filc_ents[17].name = "strcat"; filc_ents[17].entry = (void *)ent_strcat;
    filc_ents[18].name = "strncat"; filc_ents[18].entry = (void *)ent_strncat;
    filc_ents[19].name = "strnlen"; filc_ents[19].entry = (void *)ent_strnlen;
    filc_ents[20].name = "memcmp"; filc_ents[20].entry = (void *)ent_memcmp;
    filc_ents[21].name = "strndup"; filc_ents[21].entry = (void *)ent_strndup;
    filc_ents[22].name = "stpcpy"; filc_ents[22].entry = (void *)ent_stpcpy;
    filc_ents[23].name = "stpncpy"; filc_ents[23].entry = (void *)ent_stpncpy;
    filc_ents[24].name = "strspn"; filc_ents[24].entry = (void *)ent_strspn;
    filc_ents[25].name = "strcspn"; filc_ents[25].entry = (void *)ent_strcspn;
    filc_ents[26].name = "strpbrk"; filc_ents[26].entry = (void *)ent_strpbrk;
    filc_ents[27].name = "memccpy"; filc_ents[27].entry = (void *)ent_memccpy;
    filc_ents[28].name = "explicit_bzero"; filc_ents[28].entry = (void *)ent_explicit_bzero;
    filc_ents[29].name = "bzero"; filc_ents[29].entry = (void *)ent_bzero;
    filc_ents[30].name = "strtok"; filc_ents[30].entry = (void *)ent_strtok;
    filc_ents[31].name = "strtok_r"; filc_ents[31].entry = (void *)ent_strtok_r;
    filc_ents[32].name = "strsep"; filc_ents[32].entry = (void *)ent_strsep;
    filc_ents[33].name = "read"; filc_ents[33].entry = (void *)ent_read;
    filc_ents[34].name = "write"; filc_ents[34].entry = (void *)ent_write;
    filc_ents[35].name = "pread"; filc_ents[35].entry = (void *)ent_pread;
    filc_ents[36].name = "pwrite"; filc_ents[36].entry = (void *)ent_pwrite;
    filc_ents[37].name = "fread"; filc_ents[37].entry = (void *)ent_fread;
    filc_ents[38].name = "fwrite"; filc_ents[38].entry = (void *)ent_fwrite;
    filc_ents[39].name = "fgets"; filc_ents[39].entry = (void *)ent_fgets;
    filc_ents[40].name = "getenv"; filc_ents[40].entry = (void *)ent_getenv;
    filc_ents[41].name = "secure_getenv"; filc_ents[41].entry = (void *)ent_secure_getenv;
    filc_ents[42].name = "setenv"; filc_ents[42].entry = (void *)ent_setenv;
    filc_ents[43].name = "unsetenv"; filc_ents[43].entry = (void *)ent_unsetenv;
    filc_ents[44].name = "putenv"; filc_ents[44].entry = (void *)ent_putenv;
    filc_ents[45].name = "open"; filc_ents[45].entry = (void *)ent_open;
    filc_ents[46].name = "fopen"; filc_ents[46].entry = (void *)ent_fopen;
    filc_ents[47].name = "freopen"; filc_ents[47].entry = (void *)ent_freopen;
    filc_ents[48].name = "fdopen"; filc_ents[48].entry = (void *)ent_fdopen;
    filc_ents[49].name = "puts"; filc_ents[49].entry = (void *)ent_puts;
    filc_ents[50].name = "fputs"; filc_ents[50].entry = (void *)ent_fputs;
    filc_ents[51].name = "fclose"; filc_ents[51].entry = (void *)ent_fclose;
    filc_ents[52].name = "fflush"; filc_ents[52].entry = (void *)ent_fflush;
    filc_ents[53].name = "close"; filc_ents[53].entry = (void *)ent_close;
    filc_ents[54].name = "lseek"; filc_ents[54].entry = (void *)ent_lseek;
    filc_ents[55].name = "mmap"; filc_ents[55].entry = (void *)ent_mmap;
    filc_ents[56].name = "munmap"; filc_ents[56].entry = (void *)ent_munmap;
    filc_ents[57].name = "mprotect"; filc_ents[57].entry = (void *)ent_mprotect;
    filc_ents[58].name = "printf"; filc_ents[58].entry = (void *)ent_printf;
    filc_ents[59].name = "fprintf"; filc_ents[59].entry = (void *)ent_fprintf;
    filc_ents[60].name = "sprintf"; filc_ents[60].entry = (void *)ent_sprintf;
    filc_ents[61].name = "snprintf"; filc_ents[61].entry = (void *)ent_snprintf;
    filc_ents[62].name = "dprintf"; filc_ents[62].entry = (void *)ent_dprintf;
    filc_ents[63].name = "vprintf"; filc_ents[63].entry = (void *)ent_vprintf;
    filc_ents[64].name = "vfprintf"; filc_ents[64].entry = (void *)ent_vfprintf;
    filc_ents[65].name = "vsprintf"; filc_ents[65].entry = (void *)ent_vsprintf;
    filc_ents[66].name = "vsnprintf"; filc_ents[66].entry = (void *)ent_vsnprintf;
    filc_ents[67].name = "fputc"; filc_ents[67].entry = (void *)ent_fputc;
    filc_ents[68].name = "putchar"; filc_ents[68].entry = (void *)ent_putchar;
    filc_ents[69].name = "putc"; filc_ents[69].entry = (void *)ent_putc;
    filc_ents[70].name = "perror"; filc_ents[70].entry = (void *)ent_perror;
    filc_ents[71].name = "qsort"; filc_ents[71].entry = (void *)ent_qsort;
    filc_ents[72].name = "bsearch"; filc_ents[72].entry = (void *)ent_bsearch;
    filc_ents[73].name = "strcasecmp"; filc_ents[73].entry = (void *)ent_strcasecmp;
    filc_ents[74].name = "strncasecmp"; filc_ents[74].entry = (void *)ent_strncasecmp;
    filc_ents[75].name = "strcasestr"; filc_ents[75].entry = (void *)ent_strcasestr;
    filc_ents[76].name = "memmem"; filc_ents[76].entry = (void *)ent_memmem;
    filc_ents[77].name = "stat"; filc_ents[77].entry = (void *)ent_stat;
    filc_ents[78].name = "lstat"; filc_ents[78].entry = (void *)ent_lstat;
    filc_ents[79].name = "fstat"; filc_ents[79].entry = (void *)ent_fstat;
    filc_ents[80].name = "strtol"; filc_ents[80].entry = (void *)ent_strtol;
    filc_ents[81].name = "strtoll"; filc_ents[81].entry = (void *)ent_strtoll;
    filc_ents[82].name = "strtoul"; filc_ents[82].entry = (void *)ent_strtoul;
    filc_ents[83].name = "strtoull"; filc_ents[83].entry = (void *)ent_strtoull;
    filc_ents[84].name = "strtof"; filc_ents[84].entry = (void *)ent_strtof;
    filc_ents[85].name = "strtod"; filc_ents[85].entry = (void *)ent_strtod;
    filc_ents[86].name = "strtold"; filc_ents[86].entry = (void *)ent_strtold;
    filc_ents[87].name = "atoi"; filc_ents[87].entry = (void *)ent_atoi;
    filc_ents[88].name = "atol"; filc_ents[88].entry = (void *)ent_atol;
    filc_ents[89].name = "atoll"; filc_ents[89].entry = (void *)ent_atoll;
    filc_ents[90].name = "atof"; filc_ents[90].entry = (void *)ent_atof;
    filc_ents[91].name = "getcwd"; filc_ents[91].entry = (void *)ent_getcwd;
    filc_ents[92].name = "sscanf"; filc_ents[92].entry = (void *)ent_sscanf;
    filc_ents[93].name = "dlopen"; filc_ents[93].entry = (void *)ent_dlopen;
    filc_ents[94].name = "dlsym"; filc_ents[94].entry = (void *)ent_dlsym;
    filc_ents[95].name = "dlclose"; filc_ents[95].entry = (void *)ent_dlclose;
    filc_ents[96].name = "dlerror"; filc_ents[96].entry = (void *)ent_dlerror;
    filc_ents[97].name = "fscanf"; filc_ents[97].entry = (void *)ent_fscanf;
    filc_ents[98].name = "scanf"; filc_ents[98].entry = (void *)ent_scanf;
    filc_ents[99].name = "vsscanf"; filc_ents[99].entry = (void *)ent_vsscanf;
    filc_ents[100].name = "vfscanf"; filc_ents[100].entry = (void *)ent_vfscanf;
    filc_ents[101].name = "vscanf"; filc_ents[101].entry = (void *)ent_vscanf;
    real_longjmp = (filc_lj_fn)dlsym(RTLD_DEFAULT, "longjmp");
    real__longjmp = (filc_lj_fn)dlsym(RTLD_DEFAULT, "_longjmp");
    real_siglongjmp = (filc_lj_fn)dlsym(RTLD_DEFAULT, "siglongjmp");
    if (!real__longjmp)
        real__longjmp = real_longjmp;
    for (i = 0; i < FILC_NENTS; i++)
        filc_ents[i].real = dlsym(RTLD_DEFAULT, filc_ents[i].name);
    /* glibc headers alias these to __isoc99_*. Taking the address
     * yields that symbol, not the plain name dlsym finds. */
    {
        static const int scan_idx[] = {92, 97, 98, 99, 100, 101};
        int k;
        for (k = 0; k < 6; k++) {
            char buf[32];
            void *p;
            snprintf(buf, sizeof buf, "__isoc99_%s", filc_ents[scan_idx[k]].name);
            p = dlsym(RTLD_DEFAULT, buf);
            if (p)
                filc_ents[scan_idx[k]].real = p;
        }
    }
}

/* ---- conservative heap GC ------------------------------------------- */

static uint32_t filc_epoch = 1;
static filc_object **filc_sorted;
static int filc_nsorted;
static filc_object **filc_work;
static int filc_work_n, filc_work_cap;

static int filc_obj_cmp(const void *a, const void *b)
{
    uintptr_t pa = (uintptr_t)(*(filc_object *const *)a)->payload;
    uintptr_t pb = (uintptr_t)(*(filc_object *const *)b)->payload;
    if (pa < pb)
        return -1;
    if (pa > pb)
        return 1;
    return 0;
}

static void filc_build_sorted(void)
{
    int b, n = 0;
    filc_object *o;
    for (b = 0; b < FILC_BUCKETS; b++)
        for (o = buckets[b]; o; o = o->next)
            n++;
    free(filc_sorted);
    filc_sorted = NULL;
    filc_nsorted = 0;
    if (!n)
        return;
    filc_sorted = (filc_object **)malloc((size_t)n * sizeof(filc_object *));
    if (!filc_sorted)
        filc_panic("out of memory");
    for (b = 0; b < FILC_BUCKETS; b++)
        for (o = buckets[b]; o; o = o->next)
            filc_sorted[filc_nsorted++] = o;
    qsort(filc_sorted, (size_t)filc_nsorted, sizeof(filc_object *), filc_obj_cmp);
}

static void filc_mark_push(filc_object *o)
{
    if (!o || o->mark == filc_epoch)
        return;
    o->mark = filc_epoch;
    if (filc_work_n >= filc_work_cap) {
        int nc = filc_work_cap ? filc_work_cap * 2 : 64;
        filc_object **p = (filc_object **)realloc(filc_work,
                                                  (size_t)nc * sizeof(filc_object *));
        if (!p)
            filc_panic("out of memory");
        filc_work = p;
        filc_work_cap = nc;
    }
    filc_work[filc_work_n++] = o;
}

static void filc_mark_word(uintptr_t w);

static void filc_trace(filc_object *o)
{
    size_t i, n;
    if (!o->aux || (o->flags & FILC_FREE))
        return;
    n = filc_naux(o);
    for (i = 0; i < n; i++) {
        if (o->aux[i])
            filc_mark_word(o->aux[i]);
    }
}

static void filc_mark_drain(void)
{
    while (filc_work_n > 0)
        filc_trace(filc_work[--filc_work_n]);
}

static void filc_mark_word(uintptr_t w)
{
    filc_object *o;
    int lo, hi;
    if (!w)
        return;
    o = filc_lookup((void *)w);
    if (o) {
        filc_mark_push(o);
        return;
    }
    if (!filc_sorted || filc_nsorted <= 0)
        return;
    lo = 0;
    hi = filc_nsorted;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        uintptr_t p = (uintptr_t)filc_sorted[mid]->payload;
        if (p <= w)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0)
        return;
    o = filc_sorted[lo - 1];
    if ((uintptr_t)o->payload <= w
        && w < (uintptr_t)o->payload + o->size)
        filc_mark_push(o);
}

static void filc_scan_range(void *lo, void *hi)
{
    uintptr_t *p, *end;
    uintptr_t a, b;
    if (!lo || !hi)
        return;
    a = (uintptr_t)lo;
    b = (uintptr_t)hi;
    if (a >= b)
        return;
    a = (a + sizeof(void *) - 1) & ~(uintptr_t)(sizeof(void *) - 1);
    b &= ~(uintptr_t)(sizeof(void *) - 1);
    p = (uintptr_t *)a;
    end = (uintptr_t *)b;
    for (; p < end; p++)
        filc_mark_word(*p);
}

static void filc_mark_nonheap(void)
{
    int b;
    filc_object *o;
    for (b = 0; b < FILC_BUCKETS; b++)
        for (o = buckets[b]; o; o = o->next)
            if (!(o->flags & FILC_HEAP))
                filc_mark_push(o);
}

static void filc_mark_live_tls(void)
{
    int i, j, depth;
    depth = call_depth;
    if (depth < 0)
        depth = 0;
    if (depth > FILC_MAX_DEPTH)
        depth = FILC_MAX_DEPTH;
    for (i = 0; i < depth; i++)
        for (j = 0; j < FILC_MAX_PTR_ARGS; j++)
            if (call_caps[i][j])
                filc_mark_word((uintptr_t)call_caps[i][j]);
    if (filc_ret_cap)
        filc_mark_word((uintptr_t)filc_ret_cap);
    for (i = 0; i < filc_ret_n && i < FILC_MAX_RET_CAPS; i++)
        if (filc_ret_caps[i])
            filc_mark_word((uintptr_t)filc_ret_caps[i]);
}

static void filc_mark_snapshot(filc_thr *t)
{
    int i, j, depth, nregs;
    uintptr_t sp;
    depth = t->call_depth;
    if (depth < 0)
        depth = 0;
    if (depth > FILC_MAX_DEPTH)
        depth = FILC_MAX_DEPTH;
    for (i = 0; i < depth; i++)
        for (j = 0; j < FILC_MAX_PTR_ARGS; j++)
            if (t->caps[i][j])
                filc_mark_word((uintptr_t)t->caps[i][j]);
    if (t->ret_cap)
        filc_mark_word((uintptr_t)t->ret_cap);
    for (i = 0; i < t->ret_n && i < FILC_MAX_RET_CAPS; i++)
        if (t->ret_caps[i])
            filc_mark_word((uintptr_t)t->ret_caps[i]);
    nregs = t->nregs;
    if (nregs < 0)
        nregs = 0;
    if (nregs > 32)
        nregs = 32;
    for (i = 0; i < nregs; i++)
        filc_mark_word(t->regs[i]);
    sp = t->sp;
    if (t->stack_lo && t->stack_hi && (void *)sp >= t->stack_lo
        && (void *)sp < t->stack_hi)
        filc_scan_range((void *)sp, t->stack_hi);
}

static void filc_scan_self(void)
{
    jmp_buf jb;
    volatile char marker;
    if (!filc_me || !filc_me->stack_hi)
        return;
    if (setjmp(jb) != 0)
        return;
    filc_scan_range((void *)&marker, filc_me->stack_hi);
    filc_scan_range((void *)jb, (char *)jb + sizeof jb);
}

static void filc_bump_epoch(void)
{
    int b;
    filc_object *o;
    filc_epoch++;
    if (filc_epoch != 0)
        return;
    filc_epoch = 1;
    for (b = 0; b < FILC_BUCKETS; b++)
        for (o = buckets[b]; o; o = o->next)
            o->mark = 0;
}

static void filc_sweep(void)
{
    int b;
    for (b = 0; b < FILC_BUCKETS; b++) {
        filc_object **pp = &buckets[b];
        filc_object *o;
        while ((o = *pp) != NULL) {
            if ((o->flags & FILC_HEAP) && o->mark != filc_epoch) {
                *pp = o->next;
                if (filc_heap_bytes >= o->alloc_size)
                    filc_heap_bytes -= o->alloc_size;
                else
                    filc_heap_bytes = 0;
                filc_freed_objs++;
                free(o->aux);
                free(o);
            } else {
                pp = &o->next;
            }
        }
    }
}

static int filc_stacks_known(void)
{
    filc_thr *t;
    if (!filc_me || !filc_me->stack_hi)
        return 0;
    for (t = filc_threads; t; t = t->next)
        if (!t->stack_hi)
            return 0;
    return 1;
}

static void filc_gc_finish_roots(int all_threads)
{
    filc_thr *t;
    filc_build_sorted();
    filc_bump_epoch();
    filc_work_n = 0;
    filc_mark_nonheap();
    filc_mark_live_tls();
    filc_scan_self();
    if (all_threads) {
        for (t = filc_threads; t; t = t->next)
            if (t != filc_me)
                filc_mark_snapshot(t);
    }
    filc_mark_drain();
    filc_sweep();
    free(filc_sorted);
    filc_sorted = NULL;
    filc_nsorted = 0;
}

static int filc_others_acked(void)
{
    filc_thr *t;
    int n = 0;
    int live;
    pthread_mutex_lock(&filc_thr_mu);
    live = filc_cnt_load(&filc_nthreads);
    for (t = filc_threads; t; t = t->next) {
        n++;
        if (t == filc_me)
            continue;
        if (!t->ack) {
            pthread_mutex_unlock(&filc_thr_mu);
            return 0;
        }
    }
    pthread_mutex_unlock(&filc_thr_mu);
    return n == live;
}

/* One realtime signal per other thread. Queued signals pile up, so this
 * runs only when the thread count changes. Threads already inside the
 * lock notice filc_stop without the signal. */
static void filc_stw_kick(void)
{
    filc_thr *t;
    pthread_mutex_lock(&filc_thr_mu);
    for (t = filc_threads; t; t = t->next) {
        if (t == filc_me || t->ack)
            continue;
        pthread_kill(t->id, FILC_STW_SIG);
    }
    pthread_mutex_unlock(&filc_thr_mu);
}

/* Stop other filc threads without holding filc_mu. Returns 1 when every
 * published thread has snapshotted. On timeout, clear stop and leak. */
static int filc_stw(void)
{
    filc_thr *t;
    int spins, seen = -1;
    if (!filc_stw_ok)
        return 0;
    filc_is_collector = 1;
    pthread_mutex_lock(&filc_thr_mu);
    for (t = filc_threads; t; t = t->next)
        t->ack = 0;
    pthread_mutex_unlock(&filc_thr_mu);
    filc_compiler_barrier();
    filc_stop = 1;
    filc_compiler_barrier();
    for (spins = 0; spins < 100000000; spins++) {
        int n;
        if (filc_others_acked() && filc_cnt_load(&filc_entering) == 0) {
            n = filc_cnt_load(&filc_nthreads);
            filc_compiler_barrier();
            if (filc_others_acked() &&
                filc_cnt_load(&filc_nthreads) == n &&
                filc_cnt_load(&filc_entering) == 0)
                return 1;
        }
        n = filc_cnt_load(&filc_nthreads);
        if (n != seen) {
            filc_stw_kick();
            seen = n;
        }
    }
    filc_stop = 0;
    filc_is_collector = 0;
    return 0;
}

static void filc_raise_threshold(void)
{
    size_t t = filc_heap_bytes * 2;
    if (t < 256 * 1024)
        t = 256 * 1024;
    filc_gc_threshold = t;
}

void filc_collect(void)
{
    if (filc_in_gc || filc_inits_running)
        return;
    filc_apply_inits();
    if (filc_in_gc || filc_inits_running)
        return;
    filc_in_gc = 1;
    filc_lock();
    if (filc_cnt_load(&filc_nthreads) <= 1
        && filc_cnt_load(&filc_entering) == 0 && filc_stacks_known()) {
        filc_gc_finish_roots(0);
        filc_unlock();
    } else {
        int known;
        filc_unlock();
        if (filc_stw()) {
            filc_lock();
            known = filc_stacks_known();
            if (known)
                filc_gc_finish_roots(1);
            filc_stop = 0;
            filc_unlock();
            filc_is_collector = 0;
        }
    }
    filc_raise_threshold();
    filc_in_gc = 0;
}

size_t filc_freed_count(void)
{
    return filc_freed_objs;
}

static void filc_maybe_gc(void)
{
    if (filc_in_gc || filc_inits_running)
        return;
    if (!filc_inits_done)
        filc_apply_inits();
    if (filc_in_gc || filc_inits_running)
        return;
    if (filc_heap_bytes >= filc_gc_threshold)
        filc_collect();
}
