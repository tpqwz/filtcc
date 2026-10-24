struct S { int *p; int n; };
int read_s(struct S s) {
    int v = *s.p + s.n;
    *s.p = 9;
    return v;
}
int main(void) {
    int x = 7;
    struct S s;
    s.p = &x;
    s.n = 4;
    if (read_s(s) != 11)
        return 1;
    if (x != 9)
        return 2;
    return 0;
}
