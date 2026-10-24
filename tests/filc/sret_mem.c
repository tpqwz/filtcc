struct S { int *a, *b, *c; };
struct S make(int *a, int *b, int *c) {
    struct S s;
    s.a = a;
    s.b = b;
    s.c = c;
    return s;
}
int main(void) {
    int x = 1, y = 2, z = 3;
    struct S s = make(&x, &y, &z);
    if (*s.a + *s.b + *s.c != 6)
        return 1;
    return 0;
}
