struct S { int a, b, c, d, e, f; };
int sum(struct S s) { return s.a + s.b + s.c + s.d + s.e + s.f; }
int main(void) {
    struct S s;
    s.a = 1;
    s.b = 2;
    s.c = 3;
    s.d = 4;
    s.e = 5;
    s.f = 6;
    if (sum(s) != 21)
        return 1;
    return 0;
}
