struct S { int *p; int n; };
int read_s(struct S s, int k) { return *s.p + s.n + k; }
int main(void) {
    int x = 7;
    struct S s;
    s.p = &x;
    s.n = 4;
    if (read_s(s, 1) != 12)
        return 1;
    return 0;
}
