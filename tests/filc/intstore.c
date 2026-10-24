int main(void)
{
    int x = 11, y = 22;
    int *p = &x;
    *(long *)&p = (long)&y;
    return *p;
}
