int main(void) {
    int a[4];
    int *p = &a[1];
    a[0] = 1;
    return p[3];
}
