#include <string.h>

int main(void)
{
    char buf[] = "a,b,,c";
    char sep[] = "a,,b";
    char *save = 0;
    char *p;
    char *t;
    char *(*sr)(char *, const char *, char **) = strtok_r;
    char *(*ss)(char **, const char *) = strsep;

    t = strtok_r(buf, ",", &save);
    if (!t || t[0] != 'a' || t[1] != 0)
        return 1;
    t = strtok_r(0, ",", &save);
    if (!t || t[0] != 'b' || t[1] != 0)
        return 2;
    t = strtok_r(0, ",", &save);
    if (!t || t[0] != 'c' || t[1] != 0)
        return 3;
    if (strtok_r(0, ",", &save) != 0)
        return 4;

    p = sep;
    t = strsep(&p, ",");
    if (!t || t[0] != 'a' || t[1] != 0)
        return 5;
    t = strsep(&p, ",");
    if (!t || t[0] != 0)
        return 6;
    t = strsep(&p, ",");
    if (!t || t[0] != 'b' || t[1] != 0)
        return 7;
    if (strsep(&p, ",") != 0)
        return 8;

    {
        char one[] = "x-y";
        t = strtok(one, "-");
        if (!t || t[0] != 'x')
            return 9;
        t = strtok(0, "-");
        if (!t || t[0] != 'y' || t[1] != 0)
            return 10;
        if (strtok(0, "-") != 0)
            return 11;
    }
    {
        char only[] = "###";
        char *sv = 0;
        if (strtok_r(only, "#", &sv) != 0)
            return 12;
    }

    /* The resume pointer has to keep the original capability. */
    {
        char more[] = "red-green";
        char *sv = 0;
        t = sr(more, "-", &sv);
        if (!t || t[0] != 'r')
            return 13;
        t = sr(0, "-", &sv);
        if (!t || strcmp(t, "green") != 0)
            return 14;
    }
    {
        char more[] = "aa:bb";
        char *sp = more;
        t = ss(&sp, ":");
        if (!t || strcmp(t, "aa") != 0)
            return 15;
        t = ss(&sp, ":");
        if (!t || strcmp(t, "bb") != 0 || sp != 0)
            return 16;
    }
    return 0;
}
