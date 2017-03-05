/*
   CITS2002 Project 2 2016
   Names:             Lachlan Walling, Callum Sullivan
   Student numbers:   21951962,        21931205
 */
#ifndef STACK_H
#define STACK_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// link in a stack - stores data and pointer to next link
typedef struct _l {
    void *data;
    struct _l *next;
} Link;

// stack - stores size and pointer to first link
typedef struct {
    int size; // number of links
    Link *top;
} Stack;

extern void pushStr(char[], Stack *);
extern void pushInt(int, Stack *);

extern void *peek(Stack *);
extern int  peekInt(Stack *);

extern void *pop(Stack *);
extern int  popInt(Stack *);

extern void toStrArr(char *[], Stack *);
#endif
