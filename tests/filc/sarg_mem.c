struct S { int *a, *b, *c; };
int read_s(struct S s) { return *s.a + *s.b + *s.c; }
int main(void) {
    int x = 1, y = 2, z = 3;
    struct S s, g;
    s.a = &x;
    s.b = &y;
    s.c = &z;
    if (read_s(s) != 6)
        return 1;
    g.a = &x;
    g.b = &y;
    g.c = &z;
    if (read_s(g) != 6)
        return 2;
    return 0;
}
