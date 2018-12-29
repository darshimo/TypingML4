#include "param.h"
#include <stdio.h>

void writeFunt(Funt *);
void writeListt(Listt *);
void writeEnv(Env *);
void writeBox(Box *);
void writeType(Type *);
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
void writeAsmp(Asmp *, int);
void writeCncl(Cncl *, int);

void ind(int);
void writeRuleName(Cncl *);
void error(char *);
Box *getRootBox(Box *);


void writeTbd(Tbd *ob){
    printf("%%%d",ob->n);
    return;
}

void writeFunt(Funt *ob){
    char paren = (getRootBox(ob->box1_)->u.type_->type_type==FUNT);
    if(paren)printf("(");
    writeBox(ob->box1_);
    if(paren)printf(")");
    printf(" -> ");
    writeBox(ob->box2_);
    return;
}

void writeListt(Listt *ob){
    char paren = (getRootBox(ob->box_)->u.type_->type_type==FUNT);
    if(paren)printf("(");
    writeBox(ob->box_);
    if(paren)printf(")");
    printf(" list");
    return;
}

void writeEnv(Env *ob){
    if(ob==NULL)return;
    if(ob->prev!=NULL){
        writeEnv(ob->prev);
        printf(", ");
    }
    writeVar(ob->var_);
    printf(" : ");
    writeBox(ob->box_);
    return;
}

void writeBox(Box *ob){
    if(ob->box_type==ROOT) writeType(ob->u.type_);
    else writeBox(ob->u.prev);
    return;
}

void writeType(Type *ob){
    if(ob->type_type==INTT){
        printf("int");
    }else if(ob->type_type==BOOLT){
        printf("bool");
    }else if(ob->type_type==FUNT){
        writeFunt(ob->u.funt_);
    }else if(ob->type_type==LISTT){
        writeListt(ob->u.listt_);
    }else{
        //error("type is not defined.");
        writeTbd(ob->u.tbd_);
    }
    return;
}

void writeInt(Int *ob){
    printf("%d",ob->i);
    return;
}

void writeBool(Bool *ob){
    if(ob->b)printf("true");
    else printf("false");
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
    writeVar(ob->x);
    printf (" = ");
    writeExp(ob->exp1_);
    printf (" in ");
    writeExp(ob->exp2_);
    return;
}

void writeFun(Fun *ob){
    printf("fun ");
    writeVar(ob->x);
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
    writeVar(ob->x);
    printf(" = fun ");
    writeVar(ob->y);
    printf(" -> ");
    writeExp(ob->exp1_);
    printf(" in ");
    writeExp(ob->exp2_);
    return;
}

void writeCons(Cons *ob){
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
        writeCons(ob->u.cons_);
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

void writeCncl(Cncl *ob, int d){
    ind(d);

    writeEnv(ob->env_);
    if(ob->env_!=NULL)printf(" ");
    printf("|- ");
    writeExp(ob->exp_);
    printf(" : ");
    writeBox(ob->box_);

    printf(" by ");
    writeRuleName(ob);
    printf(" {");
    writeAsmp(ob->asmp_, d+1);
    if(ob->asmp_!=NULL)ind(d);
    printf("}");
    return;
}
