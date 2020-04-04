#ifndef __lib_h__
#define __lib_h__
#include "hbt.h"


Tnode *buildtree(char *filename);

Tnode *preorder_tr(FILE *fp, int *first, int *second, int *third);

#endif
