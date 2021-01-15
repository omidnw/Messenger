#include "main.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int file_exists(filename)
const char *filename;
{
    struct stat buffer;
    int exist = stat(filename, &buffer);
    if (exist == 0)
    {
        return 1;
    } else // -1
    {
        return 0;
    }
}

int folder_exists(foldername)
const char *foldername;
{
    struct stat sb;

    if (stat(foldername, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return 1;
    } else // -1
    {
        return 0;
    }
}
// Split String Read File Array : 
char** split(value, condition, array)
char* value;
char* condition;
char* array[];
{
	int counter=0;
	char *token = strtok(value, condition);
	while(token)
	{
		array[counter]=token;
		counter = counter + 1;
		token = strtok(NULL, ",");
	}
	return	array;
}
// Finish.

// Read File : 
void readfile(buffer, size, fileprocess)
char* buffer;
int size;
FILE* fileprocess;
{
	fgets(buffer, size, (FILE*)fileprocess);
}
//Finish.