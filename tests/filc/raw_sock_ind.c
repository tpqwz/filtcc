#include <sys/socket.h>
int main(void) {
    int (*f)(int, int, int) = socket;
    f(AF_UNIX, SOCK_STREAM, 0);
    return 0;
}
