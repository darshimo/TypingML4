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
    if(tmp==E_INT)printf("E-Int");
    else if(tmp==E_BOOL)printf("E-Bool");
    else if(tmp==E_IFT)printf("E-IfT");
    else if(tmp==E_IFF)printf("E-IfF");
    else if(tmp==E_PLUS)printf("E-Plus");
    else if(tmp==E_MINUS)printf("E-Minus");
    else if(tmp==E_TIMES)printf("E-Times");
    else if(tmp==E_LT)printf("E-Lt");
    else if(tmp==E_VAR)printf("E-Var");
    else if(tmp==E_LET)printf("E-Let");
    else if(tmp==E_FUN)printf("E-Fun");
    else if(tmp==E_APP)printf("E-App");
    else if(tmp==E_LETREC)printf("E-LetRec");
    else if(tmp==E_APPREC)printf("E-AppRec");
    else if(tmp==B_PLUS)printf("B-Plus");
    else if(tmp==B_MINUS)printf("B-Minus");
    else if(tmp==B_TIMES)printf("B-Times");
    else if(tmp==B_LT)printf("B-Lt");
    else if(tmp==E_NIL)printf("E-Nil");
    else if(tmp==E_CONS)printf("E-Cons");
    else if(tmp==E_MATCHNIL)printf("E-MatchNil");
    else printf("E-MatchCons");
    return;
}

Val *getVal(Env *eps, Var *x){
    if(eps==NULL)error("var is not found.");
    if(cmpVar(eps->var_,x)==0)return eps->val_;
    return getVal(eps->prev,x);
}
