/* Global struct: the lower is the object base, including through a field
 * and through a pointer stored in that field. */
struct S {
    int a;
    int *p;
    char tail[4];
};

static struct S g;
static int x = 42;

int main(void)
{
    int *q;
    g.a = 7;
    g.p = &x;
    g.tail[0] = 1;
    g.tail[3] = 9;
    q = g.p;
    if (g.a != 7 || *q != 42 || g.tail[0] != 1 || g.tail[3] != 9)
        return 1;
    if (q != &x)
        return 2;
    return 0;
}
