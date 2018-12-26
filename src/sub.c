#include <stdio.h>
#include "param.h"

int cmpVar(Var *, Var *);
void error(char *);

void ind(int d){
    int i;
    for(i=0;i<d;i++){
        printf("  ");
    }
    return;
}

void tree(int d){
    int i;
    for(i=0;i<d;i++){
    printf("\x1b[%dm",(i%6)+31);
        if(i<d-1)printf("│ ");
        else printf("├─");
    }
    printf("\x1b[39m");
    return;
}

void writeRuleName(Cncl *cncl_ob){
    RuleType tmp = cncl_ob->rule_type;
    if(tmp==T_INT)printf("T-Int");
    else if(tmp==T_BOOL)printf("T-Bool");
    else if(tmp==T_IF)printf("T-IfF");
    else if(tmp==T_PLUS)printf("T-Plus");
    else if(tmp==T_MINUS)printf("T-Minus");
    else if(tmp==T_TIMES)printf("T-Times");
    else if(tmp==T_LT)printf("T-Lt");
    else if(tmp==T_VAR)printf("T-Var");
    else if(tmp==T_LET)printf("T-Let");
    else if(tmp==T_FUN)printf("T-Fun");
    else if(tmp==T_APP)printf("T-App");
    else if(tmp==T_LETREC)printf("T-LetRec");
    else if(tmp==T_NIL)printf("T-Nil");
    else if(tmp==T_CONS)printf("T-Cons");
    else printf("T-Match");
    return;
}

/*
Val *getVal(Env *eps, Var *x){
    if(eps==NULL)error("var is not found.");
    if(cmpVar(eps->var_,x)==0)return eps->val_;
    return getVal(eps->prev,x);
}
*/
