#include <setjmp.h>
int main(void)
{
    jmp_buf jb;
    int a = 1;
    if (setjmp(jb) == 0) {
        int b = 2;
        if (setjmp(jb) == 0) {
            int c = 3;
            (void)c;
            longjmp(jb, 1);
        }
        if (a != 1 || b != 2)
            return 1;
        return 0;
    }
    return 2;
}
