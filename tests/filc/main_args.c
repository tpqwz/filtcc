int main(int argc, char **argv)
{
    if (argc < 1)
        return 1;
    if (!argv || !argv[0] || !argv[0][0])
        return 2;
    if (argv[argc] != 0)
        return 3;
    return 0;
}
