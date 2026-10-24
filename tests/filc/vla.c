int sum(int *p, int n)
{
    int i, s = 0;
    for (i = 0; i < n; i++)
        s += p[i];
    return s;
}

int main(void)
{
    int n, i, total;
    total = 0;
    for (n = 1; n <= 4; n++) {
        int a[n];
        for (i = 0; i < n; i++)
            a[i] = i + 1;
        if (a[n - 1] != n)
            return n;
        total += sum(a, n);
    }
    /* 1 + (1+2) + (1+2+3) + (1+2+3+4) = 20 */
    if (total != 20)
        return 10;
    return 0;
}
