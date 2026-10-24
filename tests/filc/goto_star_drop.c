int main(void)
{
    int n = 8;
    void *target = &&L;
    int *escaped = 0;
    {
        int a[n];
        a[0] = 9;
        escaped = &a[0];
        goto *target;
    }
L:
    return *escaped;
}
