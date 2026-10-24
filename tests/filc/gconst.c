int x = 7;
const struct S {
    int *p;
    int n;
} s = {&x, 1};

int main(void)
{
    struct S *q;
    if (*s.p != 7 || s.n != 1)
        return 1;
    q = (struct S *)&s;
    q->n = 2;
    return 0;
}
