struct S { int *p; int n; };
int read_s(int *q, struct S s) { return *q + *s.p + s.n; }
int main(void) {
    int x = 7, y = 1;
    int *q = &y;
    struct S s;
    s.p = &x;
    s.n = 4;
    if (read_s(q, s) != 12)
        return 1;
    return 0;
}
