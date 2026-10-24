struct S { int *p; int n; };
int read2(struct S s, int *q) { return *s.p + s.n + *q; }
int main(void) {
    int x = 7, y = 1;
    struct S s;
    s.p = &x;
    s.n = 4;
    if (read2(s, &y) != 12)
        return 1;
    return 0;
}
