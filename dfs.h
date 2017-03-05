/*
   CITS2002 Project 2 2016
   Names:             Lachlan Walling, Callum Sullivan
   Student numbers:   21951962,        21931205
 */
#ifndef DFS_H
#define DFS_H

#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <libgen.h>
#include <errno.h>
#include <unistd.h>
#include "stack.h"
#include "statexpr.h"

bool isSpecial(char *);
bool isHidden(char *);
void addPath(char* , char *, Stack *);

bool dfsHelper(char *, STAT_EXPRESSION, Stack *, Stack *, Stack *, bool, int);
extern bool dfs(char *, STAT_EXPRESSION, bool, int, Stack *);
#endif
