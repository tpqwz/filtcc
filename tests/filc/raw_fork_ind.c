#include <unistd.h>
int main(void) {
    pid_t (*f)(void) = fork;
    f();
    return 0;
}
