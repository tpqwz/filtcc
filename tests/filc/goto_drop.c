static int *p;
int main(void)
{
    int n = 4;
    int phase = 0;
L:
    if (phase == 1) {
        *p = 1;
        return 2;
    }
    {
        int a[n];
        a[0] = 7;
        p = a;
        phase = 1;
        goto L;
    }
    return 0;
}
