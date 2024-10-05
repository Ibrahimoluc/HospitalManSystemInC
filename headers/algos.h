#ifndef ALGOS
#define ALGOS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void foo();

void printIntArr(int *p, int size);

void printStrArr(char **arr, int size);

//max of an integer array
int getMax(int *arr, int size);


int swap(int *p1, int *p2);

int swapStr(char **s1, char **s2);

//bu algolarin parametleri void* yapsak tek bir func hepsine uygulanir mi?
int sortAsc(int *arr, int size);

int sortStr(char **arr, int size);

int haveInt(int *arr, int size, int key);
#endif
