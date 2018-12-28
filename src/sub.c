#include <stdio.h>
#include <stdlib.h>
#include "param.h"

int cmpVar(Var *, Var *);
Type *copyType(Type *);
Env *copyEnv(Env *);
Var *copyVar(Var *);
void writeType(Type *);
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
    else if(tmp==T_IF)printf("T-If");
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

Env *getEnv(Env *gamma, Var *x){
    if(gamma==NULL)error("var is not found.");
    if(cmpVar(gamma->var_,x)==0)return gamma;
    return getEnv(gamma->prev,x);
}

int typeIsDefined(Type *ob){
    TypeType tmp = ob->type_type;
    if(tmp==INTT||tmp==BOOLT) return 1;
    else if(tmp==FUNT) return typeIsDefined(ob->u.funt_->type1_)*typeIsDefined(ob->u.funt_->type2_);
    else if(tmp==LISTT) return typeIsDefined(ob->u.listt_->type_);
    return 0;
}

Type *integrateType(Type *ob1, Type *ob2){
    TypeType tmp1 = ob1->type_type;
    TypeType tmp2 = ob2->type_type;
    if(tmp1==TBD)return copyType(ob2);
    if(tmp2==TBD)return copyType(ob1);
    if(tmp1!=tmp2){
        writeType(ob1);
        printf("\n");
        writeType(ob2);
        printf("\n");
        error("mismatch type in integration.");
    }
    Type *ob = (Type *)malloc(sizeof(Type));
    ob->type_type = tmp1;
    if(tmp1==FUNT){
        ob->u.funt_ = (Funt *)malloc(sizeof(Funt));
        ob->u.funt_->type1_ = integrateType(ob1->u.funt_->type1_,ob2->u.funt_->type1_);
        ob->u.funt_->type2_ = integrateType(ob1->u.funt_->type2_,ob2->u.funt_->type2_);
    }else if(tmp1==LISTT){
        ob->u.listt_ = (Listt *)malloc(sizeof(Listt));
        ob->u.listt_->type_ = integrateType(ob1->u.listt_->type_,ob2->u.listt_->type_);
    }
    return ob;
}

Env *integrateEnv(Env *ob1, Env *ob2){
    if(ob1==NULL)return copyEnv(ob2);
    if(ob2==NULL)return copyEnv(ob1);
    if(cmpVar(ob1->var_,ob2->var_))error("env mismatch.");
    Env *ob = (Env *)malloc(sizeof(Env));
    ob->var_ = copyVar(ob1->var_);
    ob->type_ = integrateType(ob1->type_,ob2->type_);
    ob->prev = integrateEnv(ob1->prev,ob2->prev);
    return ob;
}
