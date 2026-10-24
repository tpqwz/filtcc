int main(void)
{
    int n = 8;
    int which = 0;
    int *p = 0;
L:
    switch (which) {
    case 0:
        which = 1;
        {
            int a[n];
            a[0] = 4;
            p = &a[0];
            goto L;
        }
    case 1:
        return *p;
    default:
        return 2;
    }
}
