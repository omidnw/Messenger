#include <stdio.h>
#include <string.h>

#ifndef TOOLKIT_H
#define TOOLKIT_H
int file_exists(const char* filename);
int folder_exists(const char* foldername);
void readfile(char* buffer, int size, FILE* fileprocess);
char** split(char* value, char* condition, char* array[]);

#endif