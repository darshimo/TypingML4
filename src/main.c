#include "param.h"
#include <stdlib.h>
#include <stdio.h>

//#define DEBUG
#ifdef DEBUG
void debugCncl(Cncl *,int);
#endif

void error(char*);
Cncl* readCncl(char*);
void derivation(Cncl*, int);
void freeCncl(Cncl *);
void writeCncl(Cncl *, int);
void replaceAll(Cncl *);


int main(int argc, char *argv[]){
    if(argc!=2)error("arg is not correct.\n");

    char *str = (char *)malloc(sizeof(char)*500);
    char *filename = (char *)malloc(sizeof(char)*20);
    sprintf(filename,"../problem/%s",argv[1]);
    FILE *fp;
    if((fp = fopen(filename,"r"))==NULL){
        printf("fopen error.");
        exit(EXIT_FAILURE);
    }
    fgets(str,500,fp);


#ifdef DEBUG
    printf("read start.\n");
#endif
    Cncl *cncl_ob = readCncl(str);
#ifdef DEBUG
    printf("read complete.\n\n");
#endif


#ifdef DEBUG
    printf("debug start.\n");
    debugCncl(cncl_ob,0);
    printf("debug complete.\n\n");
#endif


#ifdef DEBUG
    printf("derivation start.\n");
#endif
    derivation(cncl_ob,0);
#ifdef DEBUG
    printf("derivation complete.\n\n");
#endif


    replaceAll(cncl_ob);


    writeCncl(cncl_ob,0);


#ifdef DEBUG
    printf("\n\n");
    printf("free start.\n");
#endif
    freeCncl(cncl_ob);
#ifdef DEBUG
    printf("free complete.\n\n");
#endif

    return 0;
}
