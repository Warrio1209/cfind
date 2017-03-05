/*
   CITS2002 Project 2 2016
   Names:             Lachlan Walling, Callum Sullivan
   Student numbers:   21951962,        21931205
 */
#include "dfs.h"

typedef struct stat Stat;
typedef struct dirent Dirent;

// returns true if directory is . or .. otherwise false
bool isSpecial(char filename[]) {
    return strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0;
}

// returns true if file is hidden (name starts with .) otherwise false
bool isHidden(char filename[]) {
    return filename[0] == '.';
}

// joins a file's (child) name to a directory's path (parent) and push on stack (s)
void addPath(char *parent, char *child, Stack *s) {
    char newPath[strlen(parent)+strlen(child)+1];
    strcpy(newPath, parent);
    strcat(newPath, "/");
    strcat(newPath, child);
    pushStr(newPath, s);
}

// The body of the DFS
// returns false if file could not be checked
// parameters
// currPath      - directory or file to check
// se            - stat expression to check against
// todo          - stack to push next files onto
// children      - stack storing the number of children for each directory in the current path
// includeHidden - whether to include hidden files and directories
// maxDepth      - DFS cutoff depth
bool dfsHelper(char currPath[], STAT_EXPRESSION se, Stack *todo, Stack *matches, Stack *children, bool includeHidden, int maxDepth) {
    DIR *dirp;
    Dirent *dp;
    Stat statBuffer;

    // size == 0 only on first call
    if (children->size > 0) {
        // backtrack
        while (peekInt(children) == 0) {
            popInt(children);
        }
        // doing child so decrement parent's children count
        int currChildren = popInt(children);
        pushInt(currChildren-1, children);
    }

    if(stat(currPath, &statBuffer) != 0) {
            perror(currPath);
            return false;
    }

    // if is a directory and we have not reached the depth limit check the containing files
    if (S_ISDIR(statBuffer.st_mode) && (maxDepth == -1 || children->size <= maxDepth)) {
        dirp = opendir(currPath);
        if (dirp == NULL){
            perror(currPath);
            return false;
        }
        else {
            int numChildren = 0;
            while ((dp = readdir(dirp)) != NULL) {
                // never add */. or */.. to todo
                if (!isSpecial(dp->d_name)) {
                    // don't add hidden dirs/files to todo unless includeHidden is set
                    if (includeHidden || !isHidden(dp->d_name)) {
                        // add filename to current path and push onto stack
                        if (strcmp(currPath, ".") == 0) {// Only happens if dfs is called with currPath="." (and only the first time this function is called)
                            pushStr(dp->d_name, todo); // do not include "./" in file paths
                        }
                        else {
                            addPath(currPath, dp->d_name, todo);
                        }
                        numChildren++;
                    }
                }
            }
            // add new child count to children
            pushInt(numChildren, children);
            closedir(dirp);
        }
    }
    // Check if the file/directory is a match and push onto matches
    if (evaluate_stat_expression(se, basename(currPath), &statBuffer)){
            pushStr(currPath, matches);
    }
    return true;

}

// DFSs the directory structure starting at currPath to find files matching the stat expression se
// return false if not every file could be checked e.g. invalid permissions otherwise returns true
// parameters
// currPath      - path to start the dfs at
// se            - stat expression to check against
// includeHidden - whether to include hidden files and directories
// maxDepth      - DFS cutoff depth. -1 means no depth limit
// matches       - stack to push matching files to
bool dfs(char *currPath, STAT_EXPRESSION se, bool includeHidden, int maxDepth, Stack *matches) {
    bool success = true;
    Stack todo = {.size = 0, .top = NULL};
    // stack to store the number of children left for each directory in the current path
    // is needed to know if a directory is deeper or shallower
    // length of children stack is current depth
    Stack children = {.size = 0, .top = NULL};

    Stat statBuffer;
    if(stat(currPath, &statBuffer) != 0) {
            perror(currPath);
            return false;
    }

    // If given a file  a file, just check it and return
    if (S_ISREG(statBuffer.st_mode)){
        if (evaluate_stat_expression(se, currPath, &statBuffer)) {
            pushStr(currPath, matches);
        }
        return true;
    }

    success = success && dfsHelper(currPath, se, &todo, matches, &children, includeHidden, maxDepth);
    // do not include the start directory in the output
    if (matches->size > 0)
        free(pop(matches));
    
    while (todo.size > 0) {
        currPath = (char*) pop(&todo);
        success = success && dfsHelper(currPath, se, &todo, matches, &children, includeHidden, maxDepth);
        free(currPath); // Need to free string returned from pop
    }
    // free memory being used by children
    for (int i = 0 ; i<children.size; i++) {
        popInt(&children);
    }
    return success;
}
