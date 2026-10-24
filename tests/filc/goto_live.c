int main(void)
{
    int n = 8;
    int a[n];
    int b[n];
    a[0] = 11;
    a[n - 1] = 22;
    goto L;
L:
    if (a[0] != 11 || a[n - 1] != 22)
        return 1;
    b[0] = 3;
    b[n - 1] = 4;
    if (a[0] != 11 || b[0] != 3)
        return 2;
    return 0;
}
