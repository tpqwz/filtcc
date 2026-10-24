struct S { int *a; int *b; };
struct S make(int *a, int *b) {
    struct S s;
    s.a = a;
    s.b = b;
    return s;
}
int main(void) {
    int x = 2, y = 3;
    struct S s = make(&x, &y);
    if (*s.a + *s.b != 5)
        return 1;
    return 0;
}
