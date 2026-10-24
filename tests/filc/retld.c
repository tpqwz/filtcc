long double pick(void) { return 2.5L; }
int main(void)
{
    long double d = pick();
    if (d == 2.5L)
        return 42;
    return 1;
}
