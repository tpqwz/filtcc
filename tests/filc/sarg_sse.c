struct D { double a; double b; };
struct F { float a; };
int read_d(struct D d, int k) {
    if (d.a != 1.5 || d.b != 2.5)
        return 1;
    if (k != 3)
        return 2;
    return 0;
}
int read_f(struct F f, int *p) {
    if (f.a != 1.5f)
        return 3;
    if (*p != 7)
        return 4;
    return 0;
}
int main(void) {
    struct D d;
    struct F f;
    int x = 7;
    d.a = 1.5;
    d.b = 2.5;
    f.a = 1.5f;
    if (read_d(d, 3) != 0)
        return 1;
    if (read_f(f, &x) != 0)
        return 2;
    return 0;
}
