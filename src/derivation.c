#include "param.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

void error(char *);
Int *copyInt(Int *);
Bool *copyBool(Bool *);
Exp *copyExp(Exp *);
Env *copyEnv(Env *);
Type *copyType(Type *);
Var *copyVar(Var *);
int cmpVar(Var *, Var *);
Env *getEnv(Env *, Var *);
Type *integrateType(Type *, Type *);
Env *integrateEnv(Env *, Env *);
int typeIsDefined(Type *);
void freeType(Type *);
void freeEnv(Env *);

#ifdef DEBUG
void writeInt(Int *);
void writeBool(Bool *);
void writeEnv(Env *);
void writeType(Type *);
void writeFun(Fun *);
void writeApp(App *);
void writeLetRec(LetRec *);
void writeExp(Exp *);
void tree(int);
#endif

void derivation(Cncl *, int);


void T_Int(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Int: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_INT;

    cncl_ob->asmp_ = NULL;

    if(cncl_ob->type_->type_type==TBD){
        cncl_ob->type_->type_type = INTT;
    }else if(cncl_ob->type_->type_type!=INTT){
        error("error1");
    }

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Int");
    return;
}

void T_Bool(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Bool: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_BOOL;

    cncl_ob->asmp_ = NULL;

    if(cncl_ob->type_->type_type==TBD){
        cncl_ob->type_->type_type = BOOLT;
    }else if(cncl_ob->type_->type_type!=BOOLT){
        error("error2");
    }

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Bool");
    return;
}


void T_Var(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Var: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_VAR;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.var_;

    cncl_ob->asmp_ = NULL;

    Type *type_ob = integrateType(cncl_ob->type_,getEnv(gamma,x)->type_);
    freeType(cncl_ob->type_);
    freeType(getEnv(gamma,x)->type_);
    getEnv(gamma,x)->type_ = copyType(type_ob);
    cncl_ob->type_ = type_ob;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Var");
    return;
}

void T_Op(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Op: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.op_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.op_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = copyEnv(gamma);
    asmp_ob->cncl_->exp_ = copyExp(e1);
    asmp_ob->cncl_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->type_->type_type = INTT;
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
    asmp_ob->next->cncl_->exp_ = copyExp(e2);
    asmp_ob->next->cncl_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->type_->type_type = INTT;
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next->next = NULL;

    cncl_ob->asmp_ = asmp_ob;

    if(cncl_ob->exp_->u.op_->op_type==LT){
        cncl_ob->rule_type = T_LT;
        if(cncl_ob->type_->type_type==TBD){
            cncl_ob->type_->type_type = BOOLT;
        }else{
            if(cncl_ob->type_->type_type!=BOOLT)error("error4");
        }
    }else{
        if(cncl_ob->type_->type_type==TBD){
            cncl_ob->type_->type_type = INTT;
        }else{
            if(cncl_ob->type_->type_type!=INTT)error("error5");
        }
        if(cncl_ob->exp_->u.op_->op_type==PLUS){
            cncl_ob->rule_type = T_PLUS;
        }else if(cncl_ob->exp_->u.op_->op_type==MINUS){
            cncl_ob->rule_type = T_MINUS;
        }else{
            cncl_ob->rule_type = T_TIMES;
        }
    }

    Env *env_ob = integrateEnv(asmp_ob->cncl_->env_,asmp_ob->next->cncl_->env_);
    freeEnv(cncl_ob->env_);
    cncl_ob->env_ = env_ob;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Op");
    return;
}

void T_If(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-If: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_IF;

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.if_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.if_->exp2_;
    Exp *e3 = cncl_ob->exp_->u.if_->exp3_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = copyEnv(gamma);
    asmp_ob->cncl_->exp_ = copyExp(e1);
    asmp_ob->cncl_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->type_->type_type = BOOLT;
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
    asmp_ob->next->cncl_->exp_ = copyExp(e2);
    asmp_ob->next->cncl_->type_ = copyType(cncl_ob->type_);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->next->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_);
    asmp_ob->next->next->cncl_->exp_ = copyExp(e3);
    asmp_ob->next->next->cncl_->type_ = copyType(cncl_ob->type_);
    derivation(asmp_ob->next->next->cncl_,d+1);
    asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->next->next->cncl_->env_);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

    Type *type_ob = integrateType(asmp_ob->next->cncl_->type_,asmp_ob->next->next->cncl_->type_);
    freeType(cncl_ob->type_);
    cncl_ob->type_ = type_ob;

    Env *env_ob1 = integrateEnv(asmp_ob->cncl_->env_,asmp_ob->next->cncl_->env_);
    Env *env_ob2 = integrateEnv(env_ob1,asmp_ob->next->next->cncl_->env_);
    freeEnv(cncl_ob->env_);
    freeEnv(env_ob1);
    cncl_ob->env_ = env_ob2;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_If");

    return;
}

void T_Let(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Let: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_LET;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.let_->x;
    Exp *e1 = cncl_ob->exp_->u.let_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.let_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->exp_ = copyExp(e1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->next->cncl_->env_->prev = copyEnv(gamma);
    asmp_ob->next->cncl_->env_->var_ = copyVar(x);
    asmp_ob->next->cncl_->env_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->env_->type_->type_type = TBD;
    asmp_ob->next->cncl_->exp_ = copyExp(e2);
    asmp_ob->next->cncl_->type_ = copyType(cncl_ob->type_);
    derivation(asmp_ob->next->cncl_,d+1);

    asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_->prev);
    asmp_ob->cncl_->type_ = copyType(asmp_ob->next->cncl_->env_->type_);
    derivation(asmp_ob->cncl_,d+1);
    for(int i=0;i<3;i++){
        asmp_ob->next->cncl_->env_->prev = copyEnv(asmp_ob->cncl_->env_);
        asmp_ob->next->cncl_->env_->type_ = copyType(asmp_ob->cncl_->type_);
        derivation(asmp_ob->next->cncl_,d+1);
        asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_->prev);
        asmp_ob->cncl_->type_ = copyType(asmp_ob->next->cncl_->env_->type_);
        derivation(asmp_ob->cncl_,d+1);
    }

    asmp_ob->next->cncl_->env_->prev = copyEnv(asmp_ob->cncl_->env_);
    asmp_ob->next->cncl_->env_->type_ = copyType(asmp_ob->cncl_->type_);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

    Type *type_ob = integrateType(cncl_ob->type_,asmp_ob->next->cncl_->type_);
    freeType(cncl_ob->type_);
    cncl_ob->type_ = type_ob;

    Env *env_ob = integrateEnv(asmp_ob->cncl_->env_,asmp_ob->next->cncl_->env_->prev);
    freeEnv(cncl_ob->env_);
    cncl_ob->env_ = env_ob;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Let");
    return;
}

void T_Fun(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Fun: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_FUN;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.fun_->x;
    Exp *e = cncl_ob->exp_->u.fun_->exp_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->cncl_->env_->prev = copyEnv(gamma);
    asmp_ob->cncl_->env_->var_ = copyVar(x);
    asmp_ob->cncl_->exp_ = copyExp(e);
    if(cncl_ob->type_->type_type==TBD){
        asmp_ob->cncl_->env_->type_ = (Type *)malloc(sizeof(Type));
        asmp_ob->cncl_->env_->type_->type_type = TBD;
        asmp_ob->cncl_->type_ = (Type *)malloc(sizeof(Type));
        asmp_ob->cncl_->type_->type_type = TBD;
    }else if(cncl_ob->type_->type_type==FUNT){
        asmp_ob->cncl_->env_->type_ = copyType(cncl_ob->type_->u.funt_->type1_);
        asmp_ob->cncl_->type_ = copyType(cncl_ob->type_->u.funt_->type2_);
    }else{
        error("error8");
    }
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

    Type *type_ob1 = (Type *)malloc(sizeof(Type));
    type_ob1->type_type = FUNT;
    type_ob1->u.funt_  = (Funt *)malloc(sizeof(Funt));
    type_ob1->u.funt_->type1_ = copyType(asmp_ob->cncl_->env_->type_);
    type_ob1->u.funt_->type2_ = copyType(asmp_ob->cncl_->type_);
    Type *type_ob2 = integrateType(type_ob1,cncl_ob->type_);
    freeType(cncl_ob->type_);
    freeType(type_ob1);
    cncl_ob->type_ = type_ob2;

    freeEnv(cncl_ob->env_);
    cncl_ob->env_ = copyEnv(asmp_ob->cncl_->env_->prev);

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Fun");
    return;
}

void T_App(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-App: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_APP;

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.app_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.app_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->exp_ = copyExp(e1);
//reverse start
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = copyEnv(gamma);
    asmp_ob->next->cncl_->exp_ = copyExp(e2);
    asmp_ob->next->cncl_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->type_->type_type = TBD;
    derivation(asmp_ob->next->cncl_,d+1);
//reverse end
    asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_);
    asmp_ob->cncl_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->type_->type_type = FUNT;
    asmp_ob->cncl_->type_->u.funt_ = (Funt *)malloc(sizeof(Funt));
    asmp_ob->cncl_->type_->u.funt_->type1_ = copyType(asmp_ob->next->cncl_->type_);
    asmp_ob->cncl_->type_->u.funt_->type2_ = copyType(cncl_ob->type_);
    derivation(asmp_ob->cncl_,d+1);
//one more
    for(int i=0;i<3;i++){
        asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
        asmp_ob->next->cncl_->type_ = copyType(asmp_ob->cncl_->type_->u.funt_->type1_);
        derivation(asmp_ob->next->cncl_,d+1);
        asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_);
        asmp_ob->cncl_->type_->u.funt_->type1_ = copyType(asmp_ob->next->cncl_->type_);
        derivation(asmp_ob->cncl_,d+1);
    }
    asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
    asmp_ob->next->cncl_->type_ = copyType(asmp_ob->cncl_->type_->u.funt_->type1_);

    asmp_ob->cncl_->type_->u.funt_->type1_ = copyType(asmp_ob->next->cncl_->type_);
    asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

    Type *type_ob = integrateType(cncl_ob->type_,asmp_ob->cncl_->type_->u.funt_->type2_);
    freeType(cncl_ob->type_);
    cncl_ob->type_ = type_ob;

    Env *env_ob = integrateEnv(asmp_ob->cncl_->env_,asmp_ob->next->cncl_->env_);
    freeEnv(cncl_ob->env_);
    cncl_ob->env_ = env_ob;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_App");
    return;
}

void T_LetRec(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-LetRec: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_LETREC;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.letrec_->x;
    Var *y = cncl_ob->exp_->u.letrec_->y;
    Exp *e1 = cncl_ob->exp_->u.letrec_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.letrec_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->cncl_->env_->prev = (Env *)malloc(sizeof(Env));
    asmp_ob->cncl_->env_->prev->var_ = copyVar(x);
    asmp_ob->cncl_->env_->var_ = copyVar(y);
    asmp_ob->cncl_->exp_ = copyExp(e1);
//reverse start
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->next->cncl_->env_->prev = copyEnv(gamma);
    asmp_ob->next->cncl_->env_->var_ = copyVar(x);
    asmp_ob->next->cncl_->env_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->env_->type_->type_type = FUNT;
    asmp_ob->next->cncl_->env_->type_->u.funt_ = (Funt *)malloc(sizeof(Funt));
    asmp_ob->next->cncl_->env_->type_->u.funt_->type1_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->env_->type_->u.funt_->type1_->type_type = TBD;
    asmp_ob->next->cncl_->env_->type_->u.funt_->type2_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->env_->type_->u.funt_->type2_->type_type = TBD;
    asmp_ob->next->cncl_->exp_ = copyExp(e2);
    asmp_ob->next->cncl_->type_ = copyType(cncl_ob->type_);
    derivation(asmp_ob->next->cncl_,d+1);
//reverse end
    asmp_ob->cncl_->env_->prev->prev = copyEnv(asmp_ob->next->cncl_->env_->prev);
    asmp_ob->cncl_->env_->prev->type_ = copyType(asmp_ob->next->cncl_->env_->type_);
    asmp_ob->cncl_->env_->type_ = copyType(asmp_ob->next->cncl_->env_->type_->u.funt_->type1_);
    asmp_ob->cncl_->type_ = copyType(asmp_ob->next->cncl_->env_->type_->u.funt_->type2_);
    derivation(asmp_ob->cncl_,d+1);
    for(int i=0;i<3;i++){
        asmp_ob->next->cncl_->env_->prev = copyEnv(asmp_ob->cncl_->env_->prev->prev);
        asmp_ob->next->cncl_->env_->type_ = copyType(asmp_ob->cncl_->env_->prev->type_);
        derivation(asmp_ob->next->cncl_,d+1);
        asmp_ob->cncl_->env_->prev->prev = copyEnv(asmp_ob->next->cncl_->env_->prev);
        asmp_ob->cncl_->env_->prev->type_ = copyType(asmp_ob->next->cncl_->env_->type_);
        asmp_ob->cncl_->env_->type_ = copyType(asmp_ob->next->cncl_->env_->type_->u.funt_->type1_);
        asmp_ob->cncl_->type_ = copyType(asmp_ob->next->cncl_->env_->type_->u.funt_->type2_);
        derivation(asmp_ob->cncl_,d+1);
    }
    asmp_ob->next->cncl_->env_->prev = copyEnv(asmp_ob->cncl_->env_->prev->prev);
    asmp_ob->next->cncl_->env_->type_ = copyType(asmp_ob->cncl_->env_->prev->type_);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

    Type *type_ob = integrateType(cncl_ob->type_,asmp_ob->next->cncl_->type_);
    freeType(cncl_ob->type_);
    cncl_ob->type_ = type_ob;

    Env *env_ob = integrateEnv(asmp_ob->cncl_->env_->prev->prev,asmp_ob->next->cncl_->env_->prev);
    freeEnv(cncl_ob->env_);
    cncl_ob->env_ = env_ob;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_LetRec");
    return;
}



void T_Nil(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Nil: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_NIL;
    cncl_ob->asmp_ = NULL;
    if(cncl_ob->type_->type_type!=LISTT)error("Nil is not list.");
    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Nil");
    return;
}

void T_Cons(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Cons: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_CONS;
    if(cncl_ob->type_->type_type==TBD){
        cncl_ob->type_ = (Type *)malloc(sizeof(Type));
        cncl_ob->type_->type_type = LISTT;
        cncl_ob->type_->u.listt_ = (Listt *)malloc(sizeof(Listt));
        cncl_ob->type_->u.listt_->type_ = (Type *)malloc(sizeof(Type));
        cncl_ob->type_->u.listt_->type_->type_type = TBD;
    }
    else if(cncl_ob->type_->type_type!=LISTT)error("Cons is not list.");

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.cons_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.cons_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = copyEnv(gamma);
    asmp_ob->cncl_->exp_ = copyExp(e1);
    asmp_ob->cncl_->type_ = copyType(cncl_ob->type_->u.listt_->type_);
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
    asmp_ob->next->cncl_->exp_ = copyExp(e2);
    asmp_ob->next->cncl_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->type_->type_type = LISTT;
    asmp_ob->next->cncl_->type_->u.listt_ = (Listt *)malloc(sizeof(Listt));
    asmp_ob->next->cncl_->type_->u.listt_->type_ = copyType(asmp_ob->cncl_->type_);
    derivation(asmp_ob->next->cncl_,d+1);
    for(int i=0;i<3;i++){
        asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_);
        asmp_ob->cncl_->type_ = copyType(asmp_ob->next->cncl_->type_->u.listt_->type_);
        derivation(asmp_ob->cncl_,d+1);
        asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
        asmp_ob->next->cncl_->type_->u.listt_->type_ = copyType(asmp_ob->cncl_->type_);
        derivation(asmp_ob->next->cncl_,d+1);
    }
    asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->cncl_->env_);
    asmp_ob->cncl_->type_ = copyType(asmp_ob->next->cncl_->type_->u.listt_->type_);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

    Type *type_ob = integrateType(asmp_ob->cncl_->type_,asmp_ob->next->cncl_->type_->u.listt_->type_);
    freeType(cncl_ob->type_);
    cncl_ob->type_ = (Type *)malloc(sizeof(Type));
    cncl_ob->type_->type_type = LISTT;
    cncl_ob->type_->u.listt_ = (Listt *)malloc(sizeof(Listt));
    cncl_ob->type_->u.listt_->type_ = type_ob;

    Env *env_ob = integrateEnv(asmp_ob->cncl_->env_,asmp_ob->next->cncl_->env_);
    freeEnv(cncl_ob->env_);
    cncl_ob->env_ = env_ob;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Cons");
    return;
}

void T_Match(Cncl *cncl_ob, int d){
#ifdef DEBUG
    tree(d);
    printf("T-Match: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeType(cncl_ob->type_);
    printf("\n");
#endif
    cncl_ob->rule_type = T_MATCH;

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.match_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.match_->exp2_;
    Var *x = cncl_ob->exp_->u.match_->x;
    Var *y = cncl_ob->exp_->u.match_->y;
    Exp *e3 = cncl_ob->exp_->u.match_->exp3_;

    //set asmp 1
    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->exp_ = copyExp(e1);
    asmp_ob->cncl_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->type_->type_type = LISTT;
    asmp_ob->cncl_->type_->u.listt_ = (Listt *)malloc(sizeof(Listt));
    asmp_ob->cncl_->type_->u.listt_->type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->type_->u.listt_->type_->type_type = TBD;
    //set asmp 2
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = copyEnv(gamma);
    asmp_ob->next->cncl_->exp_ = copyExp(e2);
    asmp_ob->next->cncl_->type_ = copyType(cncl_ob->type_);
    //set asmp 3
    asmp_ob->next->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->next->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->next->next->cncl_->env_->prev = (Env *)malloc(sizeof(Env));
    asmp_ob->next->next->cncl_->env_->prev->var_ = copyVar(x);
    //asmp_ob->next->next->cncl_->env_->prev->type_ = (Type *)malloc(sizeof(Type));
    //asmp_ob->next->next->cncl_->env_->prev->type_->type_type = TBD;
    asmp_ob->next->next->cncl_->env_->var_ = copyVar(y);
    //asmp_ob->next->next->cncl_->env_->type_ = (Type *)malloc(sizeof(Type));
    //asmp_ob->next->next->cncl_->env_->type_->type_type = TBD;
    asmp_ob->next->next->cncl_->exp_ = copyExp(e3);
    //derivation
    for(int i=0;i<3;i++){
        derivation(asmp_ob->next->cncl_,d+1);
        asmp_ob->next->next->cncl_->type_ = copyType(asmp_ob->next->cncl_->type_);
        asmp_ob->next->next->cncl_->env_->prev->prev = copyEnv(asmp_ob->next->cncl_->env_);
        asmp_ob->next->next->cncl_->env_->prev->type_ = copyType(asmp_ob->cncl_->type_->u.listt_->type_);
        asmp_ob->next->next->cncl_->env_->type_ = copyType(asmp_ob->cncl_->type_);
        derivation(asmp_ob->next->next->cncl_,d+1);
        asmp_ob->cncl_->type_ = copyType(asmp_ob->next->next->cncl_->env_->type_);
        asmp_ob->cncl_->env_ = copyEnv(asmp_ob->next->next->cncl_->env_->prev->prev);
        derivation(asmp_ob->cncl_,d+1);
        asmp_ob->next->cncl_->env_ = copyEnv(asmp_ob->cncl_->env_);
        asmp_ob->next->cncl_->type_ = copyType(asmp_ob->next->next->cncl_->type_);
    }
    asmp_ob->next->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

    Type *type_ob = integrateType(cncl_ob->type_,asmp_ob->next->next->cncl_->type_);
    freeType(cncl_ob->type_);
    cncl_ob->type_ = type_ob;

    Env *env_ob1 = integrateEnv(asmp_ob->cncl_->env_,asmp_ob->next->cncl_->env_);
    Env *env_ob2 = integrateEnv(env_ob1,asmp_ob->next->next->cncl_->env_->prev->prev);
    freeEnv(cncl_ob->env_);
    freeEnv(env_ob1);
    cncl_ob->env_ = env_ob2;

    //if(typeIsDefined(cncl_ob->type_)==0)error("error in T_Cons");
    return;
}

void derivation(Cncl *cncl_ob, int d){
    ExpType tmp = cncl_ob->exp_->exp_type;
    if(tmp == INT) T_Int(cncl_ob,d);
    else if(tmp == BOOL) T_Bool(cncl_ob,d);
    else if(tmp == VAR) T_Var(cncl_ob,d);
    else if(tmp == OP) T_Op(cncl_ob, d);
    else if(tmp == IF) T_If(cncl_ob, d);
    else if(tmp == LET) T_Let(cncl_ob,d);
    else if(tmp == FUN) T_Fun(cncl_ob,d);
    else if(tmp == APP) T_App(cncl_ob,d);
    else if(tmp == LETREC) T_LetRec(cncl_ob,d);
    else if(tmp == NIL) T_Nil(cncl_ob,d);
    else if(tmp == CONS) T_Cons(cncl_ob,d);
    else T_Match(cncl_ob,d);
    return;
}
