int main(void)
{
    int n = 8;
    int a[n];
    void *labs[2];
    a[0] = 11;
    a[n - 1] = 22;
    labs[0] = &&L;
    labs[1] = &&M;
    goto *labs[0];
M:
    if (a[0] != 11 || a[1] != 3)
        return 2;
    return 0;
L:
    if (a[0] != 11 || a[n - 1] != 22)
        return 1;
    a[1] = 3;
    goto *labs[1];
}
