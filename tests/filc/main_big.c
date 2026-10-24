struct Big { int a[8]; };
struct Big main(void)
{
    struct Big b;
    int i;
    for (i = 0; i < 8; i++)
        b.a[i] = i + 1;
    return b;
}
