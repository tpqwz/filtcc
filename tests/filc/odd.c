int main(void) {
    char c = 'z';
    char *p = &c;
    if (*p != 'z') return 1;
    *p = 'q';
    if (c != 'q') return 2;
    return 0;
}
