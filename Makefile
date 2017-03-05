PROJECT =  cfind
HEADERS =  main.h stack.h dfs.h statexpr.h output.h
OBJ     =  stack.o dfs.o main.o output.o

ifeq ($(shell uname), Darwin)
    C99     =  cc -std=c99
else
    C99     = gcc -std=c99
endif

CFLAGS  =  -Wall -pedantic -Werror
LDFLAGS =  -L /cslinux/adhoc/lib -L . -l statexpr-osx -lm


# may not need -lm (links math library)
$(PROJECT) : $(OBJ)
		$(C99) $(LDFLAGS) $(CFLAGS) -o $(PROJECT) $(OBJ)


%.o : %.c $(HEADERS)
		$(C99) $(CFLAGS)  -c $<

clean:
		rm -f $(PROJECT) $(OBJ)
