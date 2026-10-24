extern int x;
extern int blob[];
int *p = &x;
int *b = blob;
int main(void)
{
    if (*p != 42)
        return 1;
    if (b[0] != 9 || b[3] != 12)
        return 2;
    return 0;
}
