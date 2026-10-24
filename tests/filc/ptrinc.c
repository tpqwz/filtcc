int main(void)
{
    int a[2];
    int *p;
    a[0] = 1;
    a[1] = 2;
    p = a;
    p++;
    if (*p != 2)
        return 1;
    p--;
    if (*p != 1)
        return 2;
    return 0;
}
