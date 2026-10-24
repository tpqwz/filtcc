/* A later, smaller VLA at the same address must not keep the old size. */
int main(void)
{
    int n;
    for (n = 4; n >= 1; n--) {
        int a[n];
        a[0] = n;
        if (n == 1) {
            a[1] = 5;
            return 2;
        }
    }
    return 0;
}
