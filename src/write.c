#include "param.h"
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
void writeAsmp(Asmp *, int);
void writeInfr(Infr *);
void writeEval(Eval *);
void writeCncl(Cncl *, int);

void ind(int);
void writeRuleName(Cncl *);


void writeInt(Int *ob){
    printf("%d",ob->i);
    return;
}

void writeBool(Bool *ob){
    if(ob->b)printf("true");
    else printf("false");
    return;
}

void writeClsr(Clsr *ob){
    printf("(");
    writeEnv(ob->env_);
    printf(")[fun ");
    writeVar(ob->arg);
    printf(" -> ");
    writeExp(ob->exp_);
    printf("]");
    return;
}

void writeClsrRec(ClsrRec *ob){
    printf("(");
    writeEnv(ob->env_);
    printf(")[rec ");
    writeVar(ob->fun);
    printf(" = fun ");
    writeVar(ob->arg);
    printf(" -> ");
    writeExp(ob->exp_);
    printf("]");
    return;
}

void writeConsv(Consv *ob){
    char paren = 0;
    if(ob->val1_->val_type==CONS_)paren=1;

    if(paren)printf("(");
    writeVal(ob->val1_);
    if(paren)printf(")");
    printf(" :: ");
    writeVal(ob->val2_);
    return;
}

void writeEnv(Env *ob){
    if(ob==NULL)return;
    if(ob->prev!=NULL){
        writeEnv(ob->prev);
        printf(", ");
    }
    writeVar(ob->var_);
    printf(" = ");
    writeVal(ob->val_);
    return;
}

void writeVal(Val *ob){
    if(ob->val_type==INT_){
        writeInt(ob->u.int_);
    }else if(ob->val_type==BOOL_){
        writeBool(ob->u.bool_);
    }else if(ob->val_type==CLSR){
        writeClsr(ob->u.clsr_);
    }else if(ob->val_type==CLSRREC){
        writeClsrRec(ob->u.clsrrec_);
    }else if(ob->val_type==CONS_){
        writeConsv(ob->u.consv_);
    }else{
        printf("[]");
    }
    return;
}

void writeVar(Var *ob){
    printf("%s",ob->var_name);
    return;
}

void writeOp(Op *ob){
    char paren1 = 0;
    char paren2 = 0;

    if(ob->exp1_->exp_type==IF || ob->exp1_->exp_type==LET || ob->exp1_->exp_type==LETREC || ob->exp1_->exp_type==MATCH){
        paren1 = 1;
    }
    if(ob->exp2_->exp_type==IF || ob->exp2_->exp_type==LET || ob->exp2_->exp_type==LETREC || ob->exp2_->exp_type==MATCH){
        paren2 = 1;
    }
    if(ob->op_type==TIMES && ob->exp1_->exp_type==OP){
        if(ob->exp1_->u.op_->op_type==PLUS || ob->exp1_->u.op_->op_type==MINUS){
            paren1 = 1;
        }
    }
    if(ob->op_type==TIMES && ob->exp2_->exp_type==OP){
        if(ob->exp2_->u.op_->op_type==PLUS || ob->exp2_->u.op_->op_type==MINUS){
            paren2 = 1;
        }
    }

    if(paren1)printf("(");
    writeExp(ob->exp1_);
    if(paren1)printf(")");
    if(ob->op_type==PLUS){
        printf(" + ");
    }else if(ob->op_type==TIMES){
        printf(" * ");
    }else if(ob->op_type==MINUS){
        printf(" - ");
    }else{
        printf(" < ");
    }
    if(paren2)printf("(");
    writeExp(ob->exp2_);
    if(paren2)printf(")");
    return;
}

void writeIf(If *ob){
    printf("if ");
    writeExp(ob->exp1_);
    printf(" then ");
    writeExp(ob->exp2_);
    printf(" else ");
    writeExp(ob->exp3_);
    return;
}

void writeLet(Let *ob){
    printf("let ");
    writeVar(ob->var_);
    printf (" = ");
    writeExp(ob->exp1_);
    printf (" in ");
    writeExp(ob->exp2_);
    return;
}

void writeFun(Fun *ob){
    printf("fun ");
    writeVar(ob->arg);
    printf(" -> ");
    writeExp(ob->exp_);
    return;
}

void writeApp(App *ob){
    char paren1 = 0;
    char paren2 = 0;

    if(ob->exp1_->exp_type == IF || ob->exp1_->exp_type == LET || ob->exp1_->exp_type == FUN || ob->exp1_->exp_type == LETREC || ob->exp1_->exp_type == MATCH)paren1 = 1;
    if(ob->exp2_->exp_type == OP || ob->exp2_->exp_type == IF || ob->exp2_->exp_type == LET || ob->exp2_->exp_type == FUN || ob->exp2_->exp_type == LETREC || ob->exp2_->exp_type == APP || ob->exp2_->exp_type == CONS || ob->exp2_->exp_type == MATCH)paren2 = 1;

    if(paren1)printf("(");
    writeExp(ob->exp1_);
    if(paren1)printf(")");
    printf(" ");
    if(paren2)printf("(");
    writeExp(ob->exp2_);
    if(paren2)printf(")");
    return;
}

void writeLetRec(LetRec *ob){
    printf("let rec ");
    writeVar(ob->fun);
    printf(" = fun ");
    writeVar(ob->arg);
    printf(" -> ");
    writeExp(ob->exp1_);
    printf(" in ");
    writeExp(ob->exp2_);
    return;
}

void writeConse(Conse *ob){
    char paren1 = 0;
    char paren2 = 0;
    if(ob->exp1_->exp_type==OP || ob->exp1_->exp_type==IF || ob->exp1_->exp_type==LET || ob->exp1_->exp_type==FUN || ob->exp1_->exp_type==APP || ob->exp1_->exp_type==LETREC || ob->exp1_->exp_type==CONS)paren1 = 1;
    if(ob->exp2_->exp_type==OP || ob->exp2_->exp_type==IF || ob->exp2_->exp_type==LET || ob->exp2_->exp_type==FUN || ob->exp2_->exp_type==APP || ob->exp2_->exp_type==LETREC)paren2 = 1;

    if(paren1)printf("(");
    writeExp(ob->exp1_);
    if(paren1)printf(")");
    printf(" :: ");
    if(paren2)printf("(");
    writeExp(ob->exp2_);
    if(paren2)printf(")");
    return;
}

void writeMatch(Match *ob){
    printf("match ");
    writeExp(ob->exp1_);
    printf(" with [] -> ");
    writeExp(ob->exp2_);
    printf(" | ");
    writeVar(ob->x);
    printf(" :: ");
    writeVar(ob->y);
    printf(" -> ");
    writeExp(ob->exp3_);
    return;
}

void writeExp(Exp *ob){
    if(ob->exp_type==INT){
        writeInt(ob->u.int_);
    }else if(ob->exp_type==BOOL){
        writeBool(ob->u.bool_);
    }else if(ob->exp_type==VAR){
        writeVar(ob->u.var_);
    }else if(ob->exp_type==OP){
        writeOp(ob->u.op_);
    }else if(ob->exp_type==IF){
        writeIf(ob->u.if_);
    }else if(ob->exp_type==LET){
        writeLet(ob->u.let_);
    }else if(ob->exp_type==FUN){
        writeFun(ob->u.fun_);
    }else if(ob->exp_type==APP){
        writeApp(ob->u.app_);
    }else if(ob->exp_type==LETREC){
        writeLetRec(ob->u.letrec_);
    }else if(ob->exp_type==CONS){
        writeConse(ob->u.conse_);
    }else if(ob->exp_type==MATCH){
        writeMatch(ob->u.match_);
    }else{
        printf("[]");
    }
    return;
}

void writeAsmp(Asmp *ob, int d){
    if(ob==NULL)return;
    printf("\n");
    writeCncl(ob->cncl_, d);
    if(ob->next!=NULL)printf(";");
    else printf("\n");
    writeAsmp(ob->next, d);
    return;
}

void writeInfr(Infr *ob){
    printf("%d",ob->int1);
    InfrOpType tmp = ob->infr_type;
    if(tmp == PLUS)printf(" plus ");
    else if(tmp == MINUS)printf(" minus ");
    else if(tmp == TIMES)printf(" times ");
    else printf(" less than ");
    printf("%d",ob->int2);
    printf(" is ");
    writeVal(ob->val_);
    return;
}

void writeEval(Eval *ob){
    writeEnv(ob->env_);
    if(ob->env_!=NULL)printf(" ");
    printf("|- ");
    writeExp(ob->exp_);
    printf(" evalto ");
    writeVal(ob->val_);
    return;
}

void writeCncl(Cncl *ob, int d){
    ind(d);
    if(ob->cncl_type==INFR)writeInfr(ob->u.infr_);
    else writeEval(ob->u.eval_);
    printf(" by ");
    writeRuleName(ob);
    printf(" {");
    writeAsmp(ob->asmp_, d+1);
    if(ob->asmp_!=NULL)ind(d);
    printf("}");
    return;
}
