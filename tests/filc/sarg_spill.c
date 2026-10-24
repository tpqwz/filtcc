struct S { int *p; int n; };
int read_s(int a, int b, int c, int d, int e, struct S s) {
    return a + b + c + d + e + *s.p + s.n;
}
int main(void) {
    int x = 7;
    struct S s;
    s.p = &x;
    s.n = 4;
    if (read_s(1, 1, 1, 1, 1, s) != 16)
        return 1;
    return 0;
}
