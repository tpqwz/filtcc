struct S { int *a; int *b; };
int read_s(struct S s) { return *s.a + *s.b; }
int main(void) {
    int x = 2, y = 3;
    struct S s;
    s.a = &x;
    s.b = &y;
    if (read_s(s) != 5)
        return 1;
    return 0;
}
