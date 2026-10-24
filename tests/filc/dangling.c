int *bad(void) {
    int x = 4;
    return &x;
}
int main(void) {
    return *bad();
}
