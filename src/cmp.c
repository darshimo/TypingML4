#include <string.h>
#include "param.h"

//#define DEBUG

int cmpInt(Int *,Int *);
int cmpBool(Bool *,Bool *);
int cmpClsr(Clsr *,Clsr *);
int cmpClsrRec(ClsrRec *,ClsrRec *);
int cmpConsv(Consv *, Consv *);
int cmpEnv(Env *,Env *);
int cmpVal(Val *,Val *);
int cmpVar(Var *,Var *);
int cmpOp(Op *,Op *);
int cmpIf(If *,If *);
int cmpLet(Let *,Let *);
int cmpFun(Fun *,Fun *);
int cmpApp(App *,App *);
int cmpLetRec(LetRec *,LetRec *);
int cmpConse(Conse *, Conse *);
int cmpMatch(Match *, Match *);
int cmpExp(Exp *,Exp *);



#ifdef DEBUG
#include <stdio.h>
void writeInt(Int *);
void writeBool(Bool *);
void writeClsr(Clsr *);
void writeClsrRec(ClsrRec *);
void writeConsv(Consv *);
void writeEnv(Env *);
void writeVal(Val *);
void writeVar(Var *);
void writeOp(Op *);
void writeIf(If *);
void writeLet(Let *);
void writeFun(Fun *);
void writeApp(App *);
void writeLetRec(LetRec *);
void writeConse(Conse *);
void writeMatch(Match *);
void writeExp(Exp *);
#endif


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

int cmpClsr(Clsr *ob1, Clsr *ob2){
#ifdef DEBUG
    printf("cmpClsr: ");
    writeClsr(ob1);
    printf(" : ");
    writeClsr(ob2);
    printf(" :\n");
#endif
    if(cmpEnv(ob1->env_,ob2->env_))return 1;
    if(cmpVar(ob1->arg,ob2->arg))return 1;
    if(cmpExp(ob1->exp_,ob2->exp_))return 1;
    return 0;
}

int cmpClsrRec(ClsrRec *ob1, ClsrRec *ob2){
#ifdef DEBUG
    printf("cmpClsrRec: ");
    writeClsrRec(ob1);
    printf(" : ");
    writeClsrRec(ob2);
    printf(" :\n");
#endif
    if(cmpEnv(ob1->env_,ob2->env_))return 1;
    if(cmpVar(ob1->fun,ob2->fun))return 1;
    if(cmpVar(ob1->arg,ob2->arg))return 1;
    if(cmpExp(ob1->exp_,ob2->exp_))return 1;
    return 0;
}

int cmpConsv(Consv *ob1, Consv *ob2){
#ifdef DEBUG
    printf("cmpConsv: ");
    writeConsv(ob1);
    printf(" : ");
    writeConsv(ob2);
    printf(" :\n");
#endif
    if(cmpVal(ob1->val1_,ob2->val1_))return 1;
    if(cmpVal(ob1->val2_,ob2->val2_))return 1;
    return 0;
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
    if(cmpVal(ob1->val_,ob2->val_))return 1;
    if(cmpEnv(ob1->prev,ob2->prev))return 1;
    return 0;
}

int cmpVal(Val *ob1, Val *ob2){
    if(ob1->val_type!=ob2->val_type)return 1;
    if(ob1->val_type==INT_)return cmpInt(ob1->u.int_,ob2->u.int_);
    if(ob1->val_type==BOOL_)return cmpBool(ob1->u.bool_,ob2->u.bool_);
    if(ob1->val_type==CLSR)return cmpClsr(ob1->u.clsr_,ob2->u.clsr_);
    if(ob1->val_type==CLSRREC)cmpClsrRec(ob1->u.clsrrec_,ob2->u.clsrrec_);
    if(ob1->val_type==CONS_)cmpConsv(ob1->u.consv_,ob2->u.consv_);
    return 0;
}

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
    if(cmpVar(ob1->var_,ob2->var_))return 1;
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
    if(cmpVar(ob1->arg,ob2->arg))return 1;
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
    if(cmpVar(ob1->fun,ob2->fun))return 1;
    if(cmpVar(ob1->arg,ob2->arg))return 1;
    if(cmpExp(ob1->exp1_,ob2->exp1_))return 1;
    if(cmpExp(ob1->exp2_,ob2->exp2_))return 1;
    return 0;
}

int cmpConse(Conse *ob1, Conse *ob2){
#ifdef DEBUG
    printf("cmpConse: ");
    writeConse(ob1);
    printf(" : ");
    writeConse(ob2);
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
    if(ob1->exp_type==CONS)cmpConse(ob1->u.conse_,ob2->u.conse_);
    if(ob1->exp_type==MATCH)cmpMatch(ob1->u.match_,ob2->u.match_);
    return 0;
}
