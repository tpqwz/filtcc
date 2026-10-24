/* File-scope and static pointer initializers must keep their lowers. */
int x = 11;
int y = 22;
int a[4] = {1, 2, 3, 4};
int *p = &x;
int *mid = &a[1];
static int *sp = &y;

struct S {
    int *p;
    int n;
};
struct S s = {&x, 3};
struct S lit = (struct S){&y, 4};

int read_at(int *q) { return *q; }
int (*fp)(int *) = read_at;

char *str = "hi";

int use_static(void)
{
    static int z = 7;
    static int *lp = &z;
    return *lp;
}

int main(void)
{
    int w = 5;
    if (*p != 11)
        return 1;
    p = &y;
    if (*p != 22)
        return 2;
    if (mid[0] != 2 || mid[2] != 4)
        return 3;
    if (*sp != 22)
        return 4;
    if (s.n != 3 || *s.p != 11)
        return 5;
    if (lit.n != 4 || *lit.p != 22)
        return 6;
    if (fp(&w) != 5)
        return 7;
    if (str[0] != 'h' || str[1] != 'i' || str[2] != 0)
        return 8;
    if (use_static() != 7)
        return 9;
    return 0;
}
