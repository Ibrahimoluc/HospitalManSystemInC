#include "inout.h"

//basic input output functions
//buraya kontroller eklenecek
int ret;

int acceptChar(char *c){
        ret = scanf(" %c", c);
	
	//clean buffer
        while ((getchar()) != '\n');
        
	return ret;
}


int acceptString(char *buffer){
        ret = scanf(" %s", buffer);
        while ((getchar()) != '\n');
        return ret;
}


int acceptInt(int *i){
        ret = scanf(" %d", i);
        while ((getchar()) != '\n');
	if(!ret) printf("not a valid input. pls input integer\n");
        return ret;
}

//buranin problemi cok, gecici duruyor.
int acceptString2(char *buffer){
	//burada mesela a2met gibi bir girisi kontrol icin gecici olarak %s al
	ret = scanf("%[a-z, A-Z]", buffer);
	//printf("ret:%d", ret);
        while ((getchar()) != '\n');
	if(!ret) printf("not a valid input. pls input only characters.\n");
	return ret;	
}

int acceptLong(long *l){
	int ret;
	ret = scanf("%ld", l);
        while ((getchar()) != '\n');
	return ret;	
}


int acceptFloat(float *f){
	int ret;
	ret = scanf("%f", f);
        while ((getchar()) != '\n');
	return ret;
}
