/* goto a label that this entry has not executed yet. The mark must
 * not be read from an uninitialized slot. */
static int precedence(int tok)
{
    switch (tok) {
    case 1:
        return 9;
    relat:
    case 2:
        return 7;
    default:
        if (tok >= 3 && tok <= 4)
            goto relat;
        return 0;
    }
}

int main(void)
{
    int i;
    int sum = 0;
    for (i = 0; i < 5; i++)
        sum += precedence(i);
    if (sum != 30)
        return 1;
    return 0;
}
