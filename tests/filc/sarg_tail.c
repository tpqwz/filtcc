struct S { int *p; int n; };
int read_s(int a, int b, struct S s, int *q) {
    return a + b + *s.p + s.n + *q;
}
int main(void) {
    int x = 7, y = 1;
    struct S s;
    s.p = &x;
    s.n = 4;
    if (read_s(1, 2, s, &y) != 15)
        return 1;
    return 0;
}
