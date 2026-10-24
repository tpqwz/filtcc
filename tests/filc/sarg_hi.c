struct S { int n; int *p; };
int read_s(struct S s) { return s.n + *s.p; }
int main(void) {
    int x = 7;
    struct S s;
    s.n = 4;
    s.p = &x;
    if (read_s(s) != 11)
        return 1;
    return 0;
}
