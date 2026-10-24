int main(void) {
    int x = 3;
    int *p = &x;
    if (*p != 3) return 1;
    *p = 8;
    if (x != 8) return 2;
    return 0;
}
