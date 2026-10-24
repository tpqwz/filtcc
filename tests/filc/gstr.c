char *s = "hi";
int main(void)
{
    if (s[0] != 'h')
        return 1;
    s[0] = 'x';
    return 0;
}
