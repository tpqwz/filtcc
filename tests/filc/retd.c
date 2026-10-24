double pick(void) { return 2.5; }
int main(void) {
    double d = pick();
    if (d == 2.5)
        return 42;
    return 1;
}
