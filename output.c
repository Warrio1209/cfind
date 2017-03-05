/*
   CITS2002 Project 2 2016
   Name(s):             Lachlan Walling, Callum Sullivan
   Student number(s):   21951962, 21931205
 */
#include "output.h"

typedef struct stat Stat;

// Compare files by path alphabetically
int nameCompare(const void * a, const void * b) {
    return strcmp(*((char **)a), *((char **)b));
}

// compare files by modification time - most recently modified first
// if modification time can't be determined sort by path alphabetically
int timeCompare(const void * a, const void * b)
{
    Stat aStatBuffer;
    // qsort calls with a void pointer pointing to the character pointer - need to cast this to a char** and dereference
    if(stat(*((char**) a), &aStatBuffer) != 0) {
        perror(*((char **) a));
        return nameCompare(a,b);
    }
    Stat bStatBuffer;
    if(stat(*((char**) b), &bStatBuffer) != 0) {
        perror(*((char **) b));
        return nameCompare(a,b);
    }
    int result = bStatBuffer.st_mtime - aStatBuffer.st_mtime;
    if (result == 0)
        return nameCompare(a,b);
    else
        return result;
    
}

//compare files by size - smallest first
// if modification time can't be determined sort by path alphabetically
int sizeCompare(const void * a, const void * b)
{
    // qsort calls with a void pointer pointing to the character pointer - need to cast this to a char** and dereference
    Stat aStatBuffer;
    if(stat(*((char**) a), &aStatBuffer) != 0) {
        perror(*((char **) a));
        return nameCompare(a,b);
    }
    Stat bStatBuffer;
    if(stat(*((char**) b), &bStatBuffer) != 0) {
        perror(*((char **) b));
        return nameCompare(a,b);
    }
    int result =  aStatBuffer.st_size - bStatBuffer.st_size;
    if (result == 0)
        return nameCompare(a,b);
    else
        return result;
    
}

// Reverses an array
void reverseArr(char *arr[], int size) {
    for (int i=0; i<size/2; i++) {
        char *temp = arr[i];
        arr[i] = arr[size-i-1];
        arr[size-i-1] = temp;
    }

}

// sorts and formats output
// returns true if successful
// on error continues outputting but returns false
bool output(Stack stack, int flags[])
{
    if (flags[COUNT]) {
        printf("%d\n", stack.size);
        return true;
    }
    bool success = true;
    // characters for long permission format
    char modes[] = {'r','w','x'};
    // permission bit masks
    int permissions[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    // array of strings to store paths for qsorting
    char *sortedOutput[stack.size];
    toStrArr(sortedOutput, &stack);

    // sort by the appropriate comparison funtion
    if (flags[TIME])
        qsort(sortedOutput, stack.size, sizeof(char*), timeCompare);
    else if (flags[SIZE])
        qsort(sortedOutput, stack.size, sizeof(char*), sizeCompare);
    else
        qsort(sortedOutput, stack.size, sizeof(char*), nameCompare);

    // reverse order if necessary
    if (flags[REVERSE]){
        reverseArr(sortedOutput, stack.size);
    }

    // print all the paths
    for (int i=0; i<stack.size; i++) {
        if (flags[LONG]) {
            // sample solution appears to be using fixed widths for the number of links and the size
            // 2 and 8 respectively
            Stat statBuffer;
            if (stat(sortedOutput[i], &statBuffer) != 0) {
                perror(sortedOutput[i]);
                success = false;
            }
            printf("%8d ", (int) statBuffer.st_ino); // inode
            printf("%c", S_ISDIR(statBuffer.st_mode)?'d':'-'); // file type
            // loop through the permission bitmasks and print the appropriate character
            for (int i = 0; i<9; i++) 
                printf("%c", statBuffer.st_mode & permissions[i]?modes[i%3]:'-');
            printf(" %2d", statBuffer.st_nlink); // number of links
            printf(" %s", getpwuid(statBuffer.st_uid)->pw_name); // get owner name
            printf(" %s", getgrgid(statBuffer.st_gid)->gr_name); // get owner group name
            printf(" %8d", (int) statBuffer.st_size); // size of file
            char *time = asctime(localtime(&statBuffer.st_mtime)); // modification time
            time[strlen(time)-1]= '\0'; // remove newline
            printf(" %s", time);
            printf(" %s\n", sortedOutput[i]); // filename
        }
        else
            printf("%s\n", sortedOutput[i]);
    }
    return success;
}

// unlinks files and directories specified in matches
// returns true on success 
// on error continues unlinking and returns false
bool deleteFiles(Stack *matches) {
    bool success = true;
    while (matches->size > 0) {
        char *currPath = (char *) pop(matches);
        Stat statBuffer;
        if(stat(currPath, &statBuffer) != 0) {
            perror(currPath);
            success = false;
        }
        // if directory use rmdir
        if (S_ISDIR(statBuffer.st_mode)) {
            if (rmdir(currPath) != 0) {
                perror(currPath);
                success = false;
            }
        }
        // if file use unlink
        else {
            if (unlink(currPath) != 0) {
                perror(currPath);
                success = false;
            }
        }
        free(currPath);
    }
    return success;
}

