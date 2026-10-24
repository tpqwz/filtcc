struct S { int *p; int n; };
struct S make(int *p) {
    struct S s;
    s.p = p;
    s.n = 4;
    return s;
}
int main(void) {
    int x = 7;
    struct S s = make(&x);
    if (*s.p + s.n != 11)
        return 1;
    return 0;
}
