int main(void)
{
    int n = 4;
    int i = 0;
loop:
    {
        int a[n];
        a[0] = i;
        a[n - 1] = i + 1;
        if (a[0] != i || a[n - 1] != i + 1)
            return 1;
        if (++i < 3)
            goto loop;
    }
    return 0;
}
