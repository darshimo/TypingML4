#include <string.h>
#include "param.h"

//#define DEBUG

int cmpVar(Var *,Var *);
/*
int cmpFunt(Funt *, Funt *);
int cmpListt(Listt *, Listt *);
int cmpEnv(Env *,Env *);
int cmpType(Type *,Type *);
int cmpInt(Int *,Int *);
int cmpBool(Bool *,Bool *);
int cmpOp(Op *,Op *);
int cmpIf(If *,If *);
int cmpLet(Let *,Let *);
int cmpFun(Fun *,Fun *);
int cmpApp(App *,App *);
int cmpLetRec(LetRec *,LetRec *);
int cmpCons(Cons *, Cons *);
int cmpMatch(Match *, Match *);
int cmpExp(Exp *,Exp *);
*/

void error(char *);


#ifdef DEBUG
#include <stdio.h>
void writeFunt(Funt *);
void writeListt(Listt *);
void writeEnv(Env *);
void writeInt(Int *);
void writeBool(Bool *);
void writeVar(Var *);
void writeOp(Op *);
void writeIf(If *);
void writeLet(Let *);
void writeFun(Fun *);
void writeApp(App *);
void writeLetRec(LetRec *);
void writeCons(Cons *);
void writeMatch(Match *);
void writeExp(Exp *);
#endif


int cmpVar(Var *ob1, Var *ob2){
#ifdef DEBUG
    printf("cmpVar: ");
    writeVar(ob1);
    printf(" : ");
    writeVar(ob2);
    printf(" :\n");
#endif
    return strcmp(ob1->var_name, ob2->var_name);
}

/*
int cmpFunt(Funt *ob1, Funt *ob2){
#ifdef DEBUG
    printf("cmpFunt: ");
    writeFunt(ob1);
    printf(" : ");
    writeFunt(ob2);
    printf(" :\n");
#endif
    if(cmpType(ob1->type1_,ob2->type1_))return 1;
    if(cmpType(ob1->type2_,ob2->type2_))return 1;
    return 0;
}

int cmpListt(Listt *ob1, Listt *ob2){
#ifdef DEBUG
    printf("cmpListt: ");
    writeListt(ob1);
    printf(" : ");
    writeListt(ob2);
    printf(" :\n");
#endif
    return cmpType(ob1->type_,ob2->type_);
}

int cmpInt(Int *ob1, Int *ob2){
#ifdef DEBUG
    printf("cmpInt: ");
    writeInt(ob1);
    printf(" : ");
    writeInt(ob2);
    printf(" :\n");
#endif
    return (ob1->i!=ob2->i);
}

int cmpBool(Bool *ob1, Bool *ob2){
#ifdef DEBUG
    printf("cmpBool: ");
    writeBool(ob1);
    printf(" : ");
    writeBool(ob2);
    printf(" :\n");
#endif
    return (ob1->b!=ob2->b);
}

int cmpEnv(Env *ob1, Env *ob2){
#ifdef DEBUG
    printf("cmpEnv: ");
    writeEnv(ob1);
    printf(" : ");
    writeEnv(ob2);
    printf(" :\n");
#endif
    if(ob1==NULL&&ob2==NULL)return 0;
    if(cmpVar(ob1->var_,ob2->var_))return 1;
    if(cmpType(ob1->type_,ob2->type_))return 1;
    if(cmpEnv(ob1->prev,ob2->prev))return 1;
    return 0;
}

int cmpType(Type *ob1, Type *ob2){
    if(ob1->type_type==TBD)error("type of ob1 is not defined.");
    if(ob2->type_type==TBD)error("type of ob2 is not defined.");
    if(ob1->type_type!=ob2->type_type)return 1;
    if(ob1->type_type==INTT){
#ifdef DEBUG
        printf("cmpIntt: intt : intt :\n");
#endif
        return 0;
    }
    if(ob1->type_type==BOOLT){
#ifdef DEBUG
        printf("cmpBoolt: boolt : boolt :\n");
#endif
        return 0;
    }
    if(ob1->type_type==FUNT)return cmpFunt(ob1->u.funt_,ob2->u.funt_);
    if(ob1->type_type==LISTT)return cmpListt(ob1->u.listt_,ob2->u.listt_);
    return 0;
}

int cmpOp(Op *ob1, Op *ob2){
#ifdef DEBUG
    printf("cmpOp: ");
    writeOp(ob1);
    printf(" : ");
    writeOp(ob2);
    printf(" :\n");
#endif
    if(ob1->op_type!=ob2->op_type)return 1;
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    return 0;
}

int cmpIf(If *ob1, If *ob2){
#ifdef DEBUG
    printf("cmpIf: ");
    writeIf(ob1);
    printf(" : ");
    writeIf(ob2);
    printf(" :\n");
#endif
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    if(cmpExp(ob1->exp3_,ob2->exp3_))return 1;
    return 0;
}

int cmpLet(Let *ob1, Let *ob2){
#ifdef DEBUG
    printf("cmpLet: ");
    writeLet(ob1);
    printf(" : ");
    writeLet(ob2);
    printf(" :\n");
#endif
    if(cmpVar(ob1->x,ob2->x))return 1;
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    return 0;
}

int cmpFun(Fun *ob1, Fun *ob2){
#ifdef DEBUG
    printf("cmpFun: ");
    writeFun(ob1);
    printf(" : ");
    writeFun(ob2);
    printf(" :\n");
#endif
    if(cmpVar(ob1->x,ob2->x))return 1;
    if(cmpExp(ob1->exp_,ob2->exp_))return 1;
    return 0;
}

int cmpApp(App *ob1, App *ob2){
#ifdef DEBUG
    printf("cmpApp: ");
    writeApp(ob1);
    printf(" : ");
    writeApp(ob2);
    printf(" :\n");
#endif
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    return 0;
}

int cmpLetRec(LetRec *ob1, LetRec *ob2){
#ifdef DEBUG
    printf("cmpLetRec: ");
    writeLetRec(ob1);
    printf(" : ");
    writeLetRec(ob2);
    printf(" :\n");
#endif
    if(cmpVar(ob1->x,ob2->x))return 1;
    if(cmpVar(ob1->y,ob2->y))return 1;
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    return 0;
}

int cmpCons(Cons *ob1, Cons *ob2){
#ifdef DEBUG
    printf("cmpCons: ");
    writeCons(ob1);
    printf(" : ");
    writeCons(ob2);
    printf(" :\n");
#endif
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    return 0;
}

int cmpMatch(Match *ob1, Match *ob2){
#ifdef DEBUG
    printf("cmpMatch: ");
    writeMatch(ob1);
    printf(" : ");
    writeMatch(ob2);
    printf(" :\n");
#endif
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    if(cmpVar(ob1->x,ob2->x))return 1;
    if(cmpVar(ob1->y,ob2->y))return 1;
    if(cmpExp(ob1->exp3_,ob2->exp3_))return 1;
    return 0;
}

int cmpExp(Exp *ob1, Exp *ob2){
    if(ob1->exp_type!=ob2->exp_type)return 1;
    if(ob1->exp_type==INT)return cmpInt(ob1->u.int_,ob2->u.int_);
    if(ob1->exp_type==BOOL)return cmpBool(ob1->u.bool_,ob2->u.bool_);
    if(ob1->exp_type==VAR)return cmpVar(ob1->u.var_,ob2->u.var_);
    if(ob1->exp_type==OP)return cmpOp(ob1->u.op_,ob2->u.op_);
    if(ob1->exp_type==IF)return cmpIf(ob1->u.if_,ob2->u.if_);
    if(ob1->exp_type==LET)return cmpLet(ob1->u.let_,ob2->u.let_);
    if(ob1->exp_type==FUN)return cmpFun(ob1->u.fun_,ob2->u.fun_);
    if(ob1->exp_type==APP)return cmpApp(ob1->u.app_,ob2->u.app_);
    if(ob1->exp_type==LETREC)cmpLetRec(ob1->u.letrec_,ob2->u.letrec_);
    if(ob1->exp_type==CONS)cmpCons(ob1->u.cons_,ob2->u.cons_);
    if(ob1->exp_type==MATCH)cmpMatch(ob1->u.match_,ob2->u.match_);
    return 0;
}
*/
