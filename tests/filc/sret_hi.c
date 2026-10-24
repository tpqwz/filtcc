struct S { int n; int *p; };
struct S make(int *p) {
    struct S s;
    s.n = 4;
    s.p = p;
    return s;
}
int main(void) {
    int x = 7;
    struct S s = make(&x);
    if (s.n + *s.p != 11)
        return 1;
    return 0;
}
