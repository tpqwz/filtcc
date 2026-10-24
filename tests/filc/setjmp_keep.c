#include <setjmp.h>
int main(void)
{
    jmp_buf jb;
    int x = 5;
    if (setjmp(jb) == 0) {
        int y = 9;
        (void)y;
        longjmp(jb, 1);
    }
    if (x != 5)
        return 1;
    return 0;
}
