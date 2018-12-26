#include "param.h"
#include <stdlib.h>
#include <string.h>

#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

Funt *copyFunt(Funt *);
Listt *copyListt(Listt *);
Env *copyEnv(Env *);
Type *copyType(Type *);
Int *copyInt(Int *);
Bool *copyBool(Bool *);
Var *copyVar(Var *);
Op *copyOp(Op *);
If *copyIf(If *);
Let *copyLet(Let *);
Fun *copyFun(Fun *);
App *copyApp(App *);
LetRec *copyLetRec(LetRec *);
Cons *copyCons(Cons *);
Match *copyMatch(Match *);
Exp *copyExp(Exp *);


Funt *copyFunt(Funt *sample){
#ifdef DEBUG
    printf("copyFunt start\n");
#endif
    Funt *ob = (Funt *)malloc(sizeof(Funt));
    ob->type1_ = copyType(sample->type1_);
    ob->type2_ = copyType(sample->type2_);
    return ob;
}

Listt *copyListt(Listt *sample){
#ifdef DEBUG
    printf("copyListt start\n");
#endif
    Listt *ob = (Listt *)malloc(sizeof(Listt));
    ob->type_ = copyType(sample->type_);
    return ob;
}

Env *copyEnv(Env *sample){
    if(sample==NULL)return NULL;
#ifdef DEBUG
    printf("copyEnv start\n");
#endif
    Env *ob = (Env *)malloc(sizeof(Env));
    ob->var_ = copyVar(sample->var_);
    ob->type_ = copyType(sample->type_);
    ob->prev = copyEnv(sample->prev);
    return ob;
}

Type *copyType(Type *sample){
#ifdef DEBUG
    printf("copyType start\n");
#endif
    Type *ob = (Type *)malloc(sizeof(Type));
    ob->type_type = sample->type_type;
    if(ob->type_type==FUNT){
        ob->u.funt_ = copyFunt(sample->u.funt_);
    }else if(ob->type_type==LISTT){
        ob->u.listt_ = copyListt(sample->u.listt_);
    }
    return ob;
}

Int *copyInt(Int *sample){
#ifdef DEBUG
    printf("copyInt start\n");
#endif
    Int *ob = (Int *)malloc(sizeof(Int));
    ob->i = sample->i;
    return ob;
}

Bool *copyBool(Bool *sample){
#ifdef DEBUG
    printf("copyBool start\n");
#endif
    Bool *ob = (Bool *)malloc(sizeof(Bool));
    ob->b = sample->b;
    return ob;
}

Var *copyVar(Var *sample){
#ifdef DEBUG
    printf("copyVar start\n");
#endif
    Var *ob = (Var *)malloc(sizeof(Var));
    ob->var_name = (char *)malloc(sizeof(char)*(strlen(sample->var_name)+1));
    strcpy(ob->var_name, sample->var_name);
    return ob;
};

Op *copyOp(Op *sample){
#ifdef DEBUG
    printf("copyOp start\n");
#endif
    Op *ob = (Op *)malloc(sizeof(Op));
    ob->op_type = sample->op_type;
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

If *copyIf(If *sample){
#ifdef DEBUG
    printf("copyIf start\n");
#endif
    If *ob = (If *)malloc(sizeof(If));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    ob->exp3_ = copyExp(sample->exp3_);
    return ob;
}

Let *copyLet(Let *sample){
#ifdef DEBUG
    printf("copyLet start\n");
#endif
    Let *ob = (Let *)malloc(sizeof(Let));
    ob->x = copyVar(sample->x);
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

Fun *copyFun(Fun *sample){
#ifdef DEBUG
    printf("copyFun start\n");
#endif
    Fun *ob = (Fun *)malloc(sizeof(Fun));
    ob->x = copyVar(sample->x);
    ob->exp_ = copyExp(sample->exp_);
    return ob;
}

App *copyApp(App *sample){
#ifdef DEBUG
    printf("copyApp start\n");
#endif
    App *ob = (App *)malloc(sizeof(App));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

LetRec *copyLetRec(LetRec *sample){
#ifdef DEBUG
    printf("copyLetRec start\n");
#endif
    LetRec *ob = (LetRec *)malloc(sizeof(LetRec));
    ob->x = copyVar(sample->x);
    ob->y = copyVar(sample->y);
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

Cons *copyCons(Cons *sample){
#ifdef DEBUG
    printf("copyCons start\n");
#endif
    Cons *ob = (Cons *)malloc(sizeof(Cons));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->exp2_ = copyExp(sample->exp2_);
    return ob;
}

Match *copyMatch(Match *sample){
#ifdef DEBUG
    printf("copyMatch start\n");
#endif
    Match *ob = (Match *)malloc(sizeof(Match));
    ob->exp1_ = copyExp(sample->exp1_);
    ob->x = copyVar(sample->x);
    ob->y = copyVar(sample->y);
    ob->exp2_ = copyExp(sample->exp2_);
    ob->exp3_ = copyExp(sample->exp3_);
    return ob;
}

Exp *copyExp(Exp *sample){
#ifdef DEBUG
    printf("copyExp start\n");
#endif
    Exp *ob = (Exp *)malloc(sizeof(Exp));
    ob->exp_type = sample->exp_type;
    if(ob->exp_type==INT)ob->u.int_ = copyInt(sample->u.int_);
    else if(ob->exp_type==BOOL)ob->u.bool_ = copyBool(sample->u.bool_);
    else if(ob->exp_type==VAR)ob->u.var_ = copyVar(sample->u.var_);
    else if(ob->exp_type==OP)ob->u.op_ = copyOp(sample->u.op_);
    else if(ob->exp_type==IF)ob->u.if_ = copyIf(sample->u.if_);
    else if(ob->exp_type==LET)ob->u.let_ = copyLet(sample->u.let_);
    else if(ob->exp_type==FUN)ob->u.fun_ = copyFun(sample->u.fun_);
    else if(ob->exp_type==APP)ob->u.app_ = copyApp(sample->u.app_);
    else if(ob->exp_type==LETREC)ob->u.letrec_ = copyLetRec(sample->u.letrec_);
    else if(ob->exp_type==CONS)ob->u.cons_ = copyCons(sample->u.cons_);
    else if(ob->exp_type==MATCH)ob->u.match_ = copyMatch(sample->u.match_);
    return ob;
}
