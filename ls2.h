#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE  1
#define FALSE 0
#define INDENT "    "

int ls2(char *path, char *pattern, int pat_srch);
int _ls2(struct stack_t *outstack, char *path, char *pattern, int pat_srch, int lvl);
int readpath(char *path);

#endif
