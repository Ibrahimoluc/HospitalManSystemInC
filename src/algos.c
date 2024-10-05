#include "algos.h"

//int boolean gibi
int haveInt(int *arr, int size, int key){
	for(int i=0;i<size;i++)
		if(arr[i]==key) return 1;
	return 0;
}


void printIntArr(int *p, int size){
        printf("Printing Integer Array...\n");
        for(int i=0;i<size;i++) printf("%d\n", p[i]);
}

void printStrArr(char **arr, int size){
        printf("Printing String Array...\n");
        for(int i=0;i<size;i++) printf("%s\n", arr[i]);
}

//max of an integer array
int getMax(int *arr, int size){
        int max = -99999;

        for(int i=0;i<size;i++)
                if(arr[i]>max) max=arr[i];

        return max;
}

int swap(int *p1, int *p2){
        int temp = *p1;
        *p1 = *p2;
        *p2 = temp;
}

int swapStr(char **s1, char **s2){
        char *temp = *s1;
        *s1 = *s2;
        *s2 = temp;
}

//bu algolarin parametleri void* yapsak tek bir func hepsine uygulanir mi?
int sortAsc(int *arr, int size){
        //bubble sort
        for(int i=0;i<size;i++)
                for(int j=0;j<size-1;j++)
                        if(arr[j]>arr[j+1])
                                swap(&arr[j],&arr[j+1]);

        return 0;
}


int sortStr(char **arr, int size){
        int x;
        for(int i=0;i<size;i++)
                for(int j=0;j<size-1;j++)
                        if( strcmp(arr[j],arr[j+1])>0 ){
                                swapStr(&arr[j],&arr[j+1]);
                        }
        return 0;
}
