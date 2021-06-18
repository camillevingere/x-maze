#ifndef LIB_UTIL_H
#define LIB_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *StrSub(const char *s, int pos, int len);
void decToBin(unsigned number, char messageBin[8]);

#endif
