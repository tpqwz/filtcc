#include <sys/wait.h>
int main(void)
{
    int status;
    int n;
    pid_t (*fn)(pid_t, int *, int);
    status = 7;
    n = waitpid(-1, &status, WNOHANG);
    if (n != -1 || status != 7)
        return 2;
    n = waitpid(-1, 0, WNOHANG);
    if (n != -1)
        return 3;
    fn = waitpid;
    n = fn(-1, 0, WNOHANG);
    if (n != -1)
        return 4;
    return 0;
}
