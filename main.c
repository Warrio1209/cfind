/*
   CITS2002 Project 2 2016
   Names:             Lachlan Walling, Callum Sullivan
   Student numbers:   21951962, 21931205
 */
#include "main.h"

const char options[] = {'a','c','d','l','r','s','t','u'};//valid command line flags as specified in optlist and header file

//returns the index +1 of flag in options, if flag is not present in options, returns 0;
int getFlagLoc(char flag){
    for(int i = 0; i<NUM_OPTS; i++){
        if((options[i] == flag)){
            return i+1;
        }
    }
    return 0;
}

//main function, correct format for calling is cfind [flags] pathname [stat expression]
int main(int argc,char ** argv){
    int flags[NUM_OPTS] = {0}; //array specifying which flags were used, order corresponds to options array and #defines
    char * stat; //stat expression to be compiled and check files against
    char * path; //relative path of top directory of DFS
	if(argc < 2){ //if the user did not enter command line arguemts, print usage and return exit failure
	    fprintf(stderr,USAGE);
	    exit(EXIT_FAILURE);
	}
	char currArg;
    flags[DEPTH] = EMPTY;//Init depth value
	while((currArg = getopt(argc,argv,OPTLIST)) != EMPTY) {//for each flag in argv
		int pos;
		if((pos = getFlagLoc(currArg))){ //If it exists in the options array
			if(pos -1 == DEPTH){ //if it's the depth flag
                if(!strcmp(":",optarg)) {//if there is not a depth arg
                    fprintf(stderr,USAGE);
                    exit(EXIT_FAILURE);
                }
				if(atoi(optarg) < 0){ // if depth argument is less than 0
					fprintf(stderr,USAGE);
                    exit(EXIT_FAILURE);
				}
				flags[DEPTH] = atoi(optarg);
			}
			else{//If flag is not depth flip flag
				flags[pos-1] = (flags[pos-1] + 1)%2;
			}
		}
        else{//if flag is not valid
            fprintf(stderr,USAGE);
            exit(EXIT_FAILURE);
        }
	}
    switch(argc - optind) {
        case 1://if user specified path but not stat expression
            path = argv[optind];
            stat = DEFAULT_STAT;
            break;
        case 2://if user specified path and stat expression
            path = argv[optind];
            stat = argv[optind + 1];
            break;
        default://if user specifies neither or extraneous arguments
            fprintf(stderr,USAGE);
            exit(EXIT_FAILURE);
            break;
    }
    STAT_EXPRESSION statExpr = compile_stat_expression(stat);
    //init stack
    Stack matches = {.size = 0, .top = NULL};
    //call DFS with start file/directory stat expression,flags and stack (for results)  as parameters
    bool success = dfs(path,statExpr,flags[ALL],flags[DEPTH], &matches);

    if (flags[COUNT] || !flags[UNLINK]) {//if count flag was specified
        success = success && output(matches, flags);//call output function with stack and flags as parameters to handle output type, sorting and formatting
    }
    else{// if unlink flag was specified
        success = success && deleteFiles(&matches); //delete all matching files
    }

    if (!success) {
        exit(EXIT_FAILURE);
    }
    
}
