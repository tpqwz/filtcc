/* A conditional must propagate the capability stored in a pointer,
 * not the object that contains that pointer. */
static char *pick(char *a, char *b, int cond)
{
    return cond ? a : b;
}

static int same(char *buf, int cond)
{
    char *a = buf;
    char *b = buf + 1;
    char *p = cond ? a : b;
    if (p[0] != (cond ? 'c' : 'r'))
        return 1;
    return 0;
}

int main(void)
{
    char buf[8];
    char *p;
    buf[0] = 'c';
    buf[1] = 'r';
    buf[2] = 't';
    buf[3] = '1';
    buf[4] = '.';
    buf[5] = 'o';
    buf[6] = 0;
    if (same(buf, 1) || same(buf, 0))
        return 3;
    p = pick(buf, buf + 4, 1);
    if (p[0] != 'c')
        return 4;
    p = pick(buf, buf + 4, 0);
    if (p[0] != '.')
        return 5;
    return 0;
}
