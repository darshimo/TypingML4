#include "param.h"
#include <stdlib.h>

#define DEBUG
#ifdef DEBUG
#include <stdio.h>
void debugCncl(Cncl *,int);
#endif

void error(char*);
Cncl* readCncl(char*);
/*
void derivation(Cncl*, int);
void freeCncl(Cncl *);
void writeCncl(Cncl *, int);
*/


int main(int argc, char *argv[]){
    if(argc!=2)error("arg is not correct.\n");


#ifdef DEBUG
    printf("read start.\n");
#endif
    Cncl *cncl_ob = readCncl(argv[1]);
#ifdef DEBUG
    printf("read complete.\n\n");
#endif


#ifdef DEBUG
    printf("debug start.\n");
    debugCncl(cncl_ob,0);
    printf("debug complete.\n\n");
#endif


    /*
#ifdef DEBUG
    printf("derivation start.\n");
#endif
    derivation(cncl_ob,0);
#ifdef DEBUG
    printf("derivation complete.\n\n");
#endif


    writeCncl(cncl_ob,0);


#ifdef DEBUG
    printf("free start.\n");
#endif
    freeCncl(cncl_ob);
#ifdef DEBUG
    printf("free complete.\n\n");
#endif
*/

    return 0;
}
