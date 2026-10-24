struct S { int *a, *b, *c; };
int read_s(struct S s) { return s.a[1]; }
int main(void) {
    int x = 1, y = 2, z = 3;
    struct S s;
    s.a = &x;
    s.b = &y;
    s.c = &z;
    return read_s(s);
}
