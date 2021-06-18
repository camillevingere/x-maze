#include "./libutil.h"

/* Fonction pour la découpe de chaine de caractère*/
char *StrSub(const char *s, int pos, int len)
{
    char *ps = (char *)s;
    static char rs[256];
    char *prs = rs;
    int strLen = strlen(s);

    if (pos >= strLen || len == 0)
        return NULL;
    ps += pos;

    if (len == -1 || len > (strLen - pos))
        len = strLen - pos;

    int i = 0;
    while (i < len && *ps != '\0')
    {
        *prs = *ps;
        prs++;
        ps++;
        i++;
    }
    *prs = '\0';

    return rs;
}

void decToBin(unsigned number, char messageBin[8])
{
    for (int n = 0; n < 7; n++)
        messageBin[n] = (number >> n) & 1;
}