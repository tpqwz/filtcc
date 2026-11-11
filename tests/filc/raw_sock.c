#include <sys/socket.h>
int main(void) {
    socket(AF_UNIX, SOCK_STREAM, 0);
    return 0;
}
