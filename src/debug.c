#include "param.h"
#include <stdio.h>

void debugFunt(Funt *, int);
void debugListt(Listt *, int);
void debugEnv(Env *, int);
void debugType(Type *, int);
void debugInt(Int *, int);
void debugBool(Bool *, int);
void debugVar(Var *, int);
void debugOp(Op *, int);
void debugIf(If *, int);
void debugLet(Let *, int);
void debugFun(Fun *, int);
void debugApp(App *, int);
void debugLetRec(LetRec *, int);
void debugCons(Cons *, int);
void debugMatch(Match *, int);
void debugExp(Exp *, int);
void debugCncl(Cncl *, int);
void tree(int);


void debugFunt(Funt *ob, int d){
    tree(d);
    printf("funt\n");
    debugType(ob->type1_,d+1);
    debugType(ob->type2_,d+1);
    return;
}

void debugListt(Listt *ob, int d){
    tree(d);
    printf("listt\n");
    debugType(ob->type_,d+1);
    return;
}

void debugEnv(Env *ob, int d){
    if(ob==NULL){
        tree(d);
        printf("env\n");
        return;
    }
    debugEnv(ob->prev,d);
    debugVar(ob->var_,d+1);
    debugType(ob->type_,d+1);
    return;
};

void debugType(Type *ob, int d){
    if(ob->type_type==INTT){
        tree(d);
        printf("intt\n");
    }else if(ob->type_type==BOOLT){
        tree(d);
        printf("boolt\n");
    }else if(ob->type_type==FUNT){
        debugFunt(ob->u.funt_,d);
    }else if(ob->type_type==LISTT){
        debugListt(ob->u.listt_,d);
    }
    return;
};

void debugInt(Int *ob, int d){
    tree(d);
    printf("int\n");
    tree(d+1);
    printf("%d\n",ob->i);
    return;
}

void debugBool(Bool *ob, int d){
    tree(d);
    printf("bool\n");
    tree(d+1);
    if(ob->b)printf("true\n");
    else printf("false\n");
    return;
}

void debugVar(Var *ob, int d){
    tree(d);
    printf("var\n");
    tree(d+1);
    printf("%s\n",ob->var_name);
    return;
}

void debugOp(Op *ob, int d){
    tree(d);
    if(ob->op_type==PLUS)printf("op(+)\n");
    else if(ob->op_type==TIMES)printf("op(*)\n");
    else if(ob->op_type==MINUS)printf("op(-)\n");
    else printf("op(<)\n");
    debugExp(ob->exp1_,d+1);
    debugExp(ob->exp2_,d+1);
    return;
};

void debugIf(If *ob, int d){
    tree(d);
    printf("if\n");
    debugExp(ob->exp1_,d+1);
    debugExp(ob->exp2_,d+1);
    debugExp(ob->exp3_,d+1);
    return;
};

void debugLet(Let *ob, int d){
    tree(d);
    printf("let\n");
    debugVar(ob->x,d+1);
    debugExp(ob->exp1_,d+1);
    debugExp(ob->exp2_,d+1);
    return;
}

void debugFun(Fun *ob, int d){
    tree(d);
    printf("fun\n");
    debugVar(ob->x,d+1);
    debugExp(ob->exp_,d+1);
    return;
}

void debugApp(App *ob, int d){
    tree(d);
    printf("app\n");
    debugExp(ob->exp1_,d+1);
    debugExp(ob->exp2_,d+1);
    return;
}

void debugLetRec(LetRec *ob, int d){
    tree(d);
    printf("letrec\n");
    debugVar(ob->x,d+1);
    debugVar(ob->y,d+1);
    debugExp(ob->exp1_,d+1);
    debugExp(ob->exp2_,d+1);
    return;
}

void debugCons(Cons *ob, int d){
    tree(d);
    printf("cons\n");
    debugExp(ob->exp1_,d+1);
    debugExp(ob->exp2_,d+1);
    return;
}

void debugMatch(Match *ob, int d){
    tree(d);
    printf("match\n");
    debugExp(ob->exp1_,d+1);
    debugExp(ob->exp2_,d+1);
    debugVar(ob->x,d+1);
    debugVar(ob->y,d+1);
    debugExp(ob->exp3_,d+1);
    return;
}

void debugExp(Exp *ob, int d){
    tree(d);
    printf("exp\n");
    if(ob->exp_type==INT){
        debugInt(ob->u.int_,d+1);
    }else if(ob->exp_type==BOOL){
        debugBool(ob->u.bool_,d+1);
    }else if(ob->exp_type==VAR){
        debugVar(ob->u.var_,d+1);
    }else if(ob->exp_type==OP){
        debugOp(ob->u.op_,d+1);
    }else if(ob->exp_type==IF){
        debugIf(ob->u.if_,d+1);
    }else if(ob->exp_type==LET){
        debugLet(ob->u.let_,d+1);
    }else if(ob->exp_type==FUN){
        debugFun(ob->u.fun_,d+1);
    }else if(ob->exp_type==APP){
        debugApp(ob->u.app_,d+1);
    }else if(ob->exp_type==LETREC){
        debugLetRec(ob->u.letrec_,d+1);
    }else if(ob->exp_type==CONS){
        debugCons(ob->u.cons_,d+1);
    }else if(ob->exp_type==MATCH){
        debugMatch(ob->u.match_,d+1);
    }
    return;
};

void debugCncl(Cncl *ob,int d){
    tree(d);
    printf("cncl\n");
    debugEnv(ob->env_,d+1);
    debugExp(ob->exp_,d+1);
    debugType(ob->type_,d+1);
    return;
}
