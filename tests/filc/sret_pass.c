struct S { int *p; int n; };
struct S make(int *p) {
    struct S s;
    s.p = p;
    s.n = 4;
    return s;
}
int read_s(struct S s) { return *s.p + s.n; }
int main(void) {
    int x = 7;
    if (read_s(make(&x)) != 11)
        return 1;
    return 0;
}
