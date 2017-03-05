/*
   CITS2002 Project 2 2016
   Names:             Lachlan Walling, Callum Sullivan
   Student numbers:   21951962,        21931205
 */
#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "dfs.h"
#include "output.h"

//Usage string documents how program is meant to be called
#define USAGE "Usage: cfind [options] path [expression]\n"\
"where options are:\n\n"\
"  -a\tconsider all entries, even those beginning with '.'\n"\
"  -c\tjust print a count of the matching entries\n"\
"  -d N\tdescend at most N directories (N >= 0) into the given path\n"\
"  -l\tprint a long listing, sorted by pathname\n"\
"  -r\treverse the order of any sorting options\n"\
"  -s\tsort matching file entries by size\n"\
"  -t\tsort matching file entries by modification time\n"\
"  -u\tunlink matching file entries\n"

#define NUM_OPTS 8 //Number of valid flags
#define DEFAULT_STAT "1" //The stat expression defaults to 1  (true) if left unspecified
#define OPTLIST "+acd:lrstu" //the optlist used in this program's implementation of getopt
#define EMPTY -1 //Default value of depth flag, -1 not 0 as depth of depth limited search can be 0
//Indices of flags in array
#define ALL 0 //include hidden files in dfs
#define COUNT 1 //Only print count, overrides unlink and long
#define DEPTH 2 //Depth limit of DFS, unlimited if negative 1
#define LONG 3 //Print long version ala ls -li
#define REVERSE 4 //Reverse any sorting specified
#define SIZE 5 //Sort by file size, ascending, smallest first
#define TIME 6 //Sort by time last modified, most recent first
#define UNLINK 7 //Unlink all files which satisfy stat expression, don't print anything, overrides LONG

const char options[NUM_OPTS];

int getFlagLoc(char);
int main(int argc, char ** argv);
#endif
