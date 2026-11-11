#include <dirent.h>
int main(void) {
    DIR *(*f)(const char *) = opendir;
    f("/");
    return 0;
}
