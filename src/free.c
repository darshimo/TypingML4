#include "param.h"
#include <stdlib.h>

//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

void freeFunt(Funt *);
void freeListt(Listt *);
void freeEnv(Env *);
void freeType(Type *);
void freeInt(Int *);
void freeBool(Bool *);
void freeVar(Var *);
void freeOp(Op *);
void freeIf(If *);
void freeLet(Let *);
void freeFun(Fun *);
void freeApp(App *);
void freeLetRec(LetRec *);
void freeCons(Cons *);
void freeMatch(Match *);
void freeExp(Exp *);
void freeAsmp(Asmp *);
void freeCncl(Cncl *);


void freeFunt(Funt *ob){
#ifdef DEBUG
    printf("free funt\n");
#endif
    freeType(ob->type1_);
    freeType(ob->type2_);
    free(ob);
    return;
}

void freeListt(Listt *ob){
#ifdef DEBUG
    printf("free listt\n");
#endif
    freeType(ob->type_);
    free(ob);
    return;
}

void freeEnv(Env *ob){
    if(ob==NULL)return;
#ifdef DEBUG
    printf("free env\n");
#endif
    freeVar(ob->var_);
    freeType(ob->type_);
    freeEnv(ob->prev);
    free(ob);
    return;
}

void freeType(Type *ob){
#ifdef DEBUG
    printf("free type\n");
#endif
    TypeType tmp = ob->type_type;
    if(tmp==FUNT)freeFunt(ob->u.funt_);
    else if(tmp==LISTT)freeListt(ob->u.listt_);
    free(ob);
    return;
}

void freeInt(Int *ob){
#ifdef DEBUG
    printf("free int\n");
#endif
    free(ob);
    return;
}

void freeBool(Bool *ob){
#ifdef DEBUG
    printf("free bool\n");
#endif
    free(ob);
    return;
}

void freeVar(Var *ob){
#ifdef DEBUG
    printf("free var\n");
#endif
    free(ob->var_name);
    free(ob);
    return;
}

void freeOp(Op *ob){
#ifdef DEBUG
    printf("free op\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    free(ob);
    return;
}

void freeIf(If *ob){
#ifdef DEBUG
    printf("free if\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    freeExp(ob->exp3_);
    free(ob);
    return;
}

void freeLet(Let *ob){
#ifdef DEBUG
    printf("free let\n");
#endif
    freeVar(ob->x);
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    free(ob);
    return;
}

void freeFun(Fun *ob){
#ifdef DEBUG
    printf("free fun\n");
#endif
    freeVar(ob->x);
    freeExp(ob->exp_);
    free(ob);
    return;
}

void freeApp(App *ob){
#ifdef DEBUG
    printf("free app\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    free(ob);
    return;
}

void freeLetRec(LetRec *ob){
#ifdef DEBUG
    printf("free letrec\n");
#endif
    freeVar(ob->x);
    freeVar(ob->y);
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    return;
}

void freeCons(Cons *ob){
#ifdef DEBUG
    printf("free cons\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    return;
}

void freeMatch(Match *ob){
#ifdef DEBUG
    printf("free match\n");
#endif
    freeExp(ob->exp1_);
    freeExp(ob->exp2_);
    freeVar(ob->x);
    freeVar(ob->y);
    freeExp(ob->exp3_);
    return;
}

void freeExp(Exp *ob){
#ifdef DEBUG
    printf("free exp\n");
#endif
    if(ob->exp_type==INT)freeInt(ob->u.int_);
    else if(ob->exp_type==BOOL)freeBool(ob->u.bool_);
    else if(ob->exp_type==VAR)freeVar(ob->u.var_);
    else if(ob->exp_type==OP)freeOp(ob->u.op_);
    else if(ob->exp_type==IF)freeIf(ob->u.if_);
    else if(ob->exp_type==LET)freeLet(ob->u.let_);
    else if(ob->exp_type==FUN)freeFun(ob->u.fun_);
    else if(ob->exp_type==APP)freeApp(ob->u.app_);
    else if(ob->exp_type==LETREC)freeLetRec(ob->u.letrec_);
    else if(ob->exp_type==CONS)freeCons(ob->u.cons_);
    else if(ob->exp_type==MATCH)freeMatch(ob->u.match_);
    free(ob);
    return;
}

void freeAsmp(Asmp *ob){
    if(ob==NULL)return;
#ifdef DEBUG
    printf("free asmp\n");
#endif
    freeCncl(ob->cncl_);
    freeAsmp(ob->next);
    free(ob);
    return;
}

void freeCncl(Cncl *ob){
#ifdef DEBUG
    printf("free cncl\n");
#endif
    freeAsmp(ob->asmp_);
    freeEnv(ob->env_);
    freeExp(ob->exp_);
    freeType(ob->type_);
    free(ob);
    return;
}
