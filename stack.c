/*
   CITS2002 Project 2 2016
   Names:             Lachlan Walling, Callum Sullivan
   Student numbers:   21951962,        21931205
 */
#include "stack.h"

// pushes string (character array) data onto stack s
// on error print message and quit program - program may not be able to recover
void pushStr(char data[], Stack *s) {
    Link *l = malloc(sizeof(Link));
    if (l == NULL) {
        perror("stack");
        exit(EXIT_FAILURE);
    }
    l->data = (char*)malloc(sizeof(char)*(strlen(data) + 1)); // +1 for nullbyte
    if (l->data == NULL) {
        perror("stack");
        exit(EXIT_FAILURE);
    }
    strcpy(l->data, data);
	l->next = s->top;
	s->size++;
	s->top = l;
}

// pushes integer data onto stack s
// on error print message and quit program - program may not be able to recover
void pushInt(int data, Stack *s) {
    Link *l = malloc(sizeof(Link));
    if (l == NULL) {
        perror("stack");
        exit(EXIT_FAILURE);
    }
    l->data = malloc(sizeof(int));
    if (l->data == NULL) {
        perror("stack");
        exit(EXIT_FAILURE);
    }
    *((int*)l->data) = data;
	l->next = s->top;
	s->size++;
	s->top = l;
}

// generic peek function - returns pointer to data on top of stack s
void *peek(Stack *s) {
    if (s->size == 0) {
        fprintf(stderr, "Attempt to peek empty stack\n");
        exit(EXIT_FAILURE);
    }
    return s->top->data;
}

// returns the integer on the top of stack s
// if top of stack is not an integer the behaviour is defined to be undefined
int peekInt(Stack *s) {
    return *((int *) peek(s));
}

// generic pop function - returns pointer to data on top of stack s and removes from stack
// returns dynamically allocated pointer
void *pop(Stack *s) {
    if (s->size == 0) {
        fprintf(stderr, "Attempt to pop from an empty stack\n");
        exit(EXIT_FAILURE);
    }
    void *data = s->top->data;
    free(s->top);
    s->size--;
    s->top = s->top->next;
    return data;
}

// returns integer on top of stack and removes from stack
// if top of stack is not an integer the behaviour is defined to be undefined
int popInt(Stack *s) {
    int data = *((int *)s->top->data);
    free(s->top->data);
    free(s->top);
    s->top = s->top->next;
    s->size--;
    return data;
}

// makes a deep copy of the stack into an array of strings
void toStrArr(char *arr[], Stack *s) {
    Link *curr = s->top;
    for (int i=0; i< s->size; i++) {
        arr[i] = strdup(curr->data);
        curr = curr->next;
    }
}
