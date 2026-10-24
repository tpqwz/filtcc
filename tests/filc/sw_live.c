int main(void)
{
    int n = 8;
    int a[n];
    a[0] = 6;
    switch (1) {
    case 0:
        return 2;
    case 1:
        if (a[0] != 6)
            return 1;
        a[1] = 7;
        break;
    default:
        return 3;
    }
    if (a[0] != 6 || a[1] != 7)
        return 4;
    return 0;
}
