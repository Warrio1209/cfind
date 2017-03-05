/*
   CITS2002 Project 2 2016
   Names:             Lachlan Walling, Callum Sullivan
   Student numbers:   21951962,        21931205
 */
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#include "main.h"
#include "stack.h"

// number of permissions (owner read, owner write, ..., other execute)
#define PERM_LEM 9

int nameCompare(const void *, const void *);
int timeCompare(const void *, const void *);
int sizeCompare(const void *, const void *);

extern bool output(Stack, int[]);
extern bool deleteFiles(Stack *);

#endif
