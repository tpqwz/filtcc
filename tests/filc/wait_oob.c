#include <sys/wait.h>
int main(void)
{
    char b[1];
    (void)waitpid(-1, (int *)b, WNOHANG);
    return 0;
}
