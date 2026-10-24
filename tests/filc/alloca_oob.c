int main(void)
{
    char *p = alloca(8);
    p[0] = 1;
    p[7] = 2;
    p[8] = 3;
    return 0;
}
