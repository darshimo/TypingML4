#include "param.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DBG_ALL
#define DBG_DRV
#endif

void error(char *);
Box *getRootBox(Box *);
Box *getEnvBox(Env *, Var *);
void integrateBox(Box *, Box *);
int kdkd();
/*
Int *copyInt(Int *);
Bool *copyBool(Bool *);
Exp *copyExp(Exp *);
Env *copyEnv(Env *);
Type *copyType(Type *);
Var *copyVar(Var *);
int cmpVar(Var *, Var *);
Env *integrateEnv(Env *, Env *);
void freeType(Type *);
void freeEnv(Env *);
*/

#ifdef DBG_DRV
void writeInt(Int *);
void writeBool(Bool *);
void writeEnv(Env *);
void writeBox(Box *);
void writeFun(Fun *);
void writeApp(App *);
void writeLetRec(LetRec *);
void writeExp(Exp *);
void tree(int);
#endif

void derivation(Cncl *, int);


void T_Int(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Int: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_INT;

    cncl_ob->asmp_ = NULL;

    Type *tmp = getRootBox(cncl_ob->box_)->u.type_;
    if(tmp->type_type==TBD){
        free(tmp->u.tbd_);
        tmp->type_type = INTT;
    }else if(tmp->type_type!=INTT){
        error("error1");
    }

#ifdef DBG_DRV
    tree(d);
    printf("T-Int: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_Bool(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Bool: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_BOOL;

    cncl_ob->asmp_ = NULL;

    Type *tmp = getRootBox(cncl_ob->box_)->u.type_;
    if(tmp->type_type==TBD){
        free(tmp->u.tbd_);
        tmp->type_type = BOOLT;
    }else if(tmp->type_type!=BOOLT){
        error("error2");
    }

#ifdef DBG_DRV
    tree(d);
    printf("T-Bool: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}


void T_Var(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Var: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_VAR;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.var_;

    cncl_ob->asmp_ = NULL;

    Box *box_ob1 = getRootBox(cncl_ob->box_);
    Box *box_ob2 = getRootBox(getEnvBox(gamma,x));
    integrateBox(box_ob1,box_ob2);

#ifdef DBG_DRV
    tree(d);
    printf("T-Var: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_Op(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Op: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.op_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.op_->exp2_;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = gamma;
    asmp_ob->cncl_->exp_ = e1;
    asmp_ob->cncl_->box_ = (Box *)malloc(sizeof(Box));
    asmp_ob->cncl_->box_->box_type = ROOT;
    asmp_ob->cncl_->box_->u.type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->box_->u.type_->type_type = INTT;
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = gamma;
    asmp_ob->next->cncl_->exp_ = e2;
    asmp_ob->next->cncl_->box_ = (Box *)malloc(sizeof(Box));
    asmp_ob->next->cncl_->box_->box_type = ROOT;
    asmp_ob->next->cncl_->box_->u.type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->next->cncl_->box_->u.type_->type_type = INTT;
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;
    derivation(asmp_ob->cncl_,d+1);
    derivation(asmp_ob->next->cncl_,d+1);

    Type *type_ob = getRootBox(cncl_ob->box_)->u.type_;
    if(cncl_ob->exp_->u.op_->op_type==LT){
        cncl_ob->rule_type = T_LT;
        if(type_ob->type_type==TBD){
            type_ob->type_type = BOOLT;
        }else{
            if(type_ob->type_type!=BOOLT)error("error4");
        }
    }else{
        if(type_ob->type_type==TBD){
            type_ob->type_type = INTT;
        }else{
            if(type_ob->type_type!=INTT)error("error5");
        }
        if(cncl_ob->exp_->u.op_->op_type==PLUS){
            cncl_ob->rule_type = T_PLUS;
        }else if(cncl_ob->exp_->u.op_->op_type==MINUS){
            cncl_ob->rule_type = T_MINUS;
        }else{
            cncl_ob->rule_type = T_TIMES;
        }
    }

#ifdef DBG_DRV
    tree(d);
    printf("T-Op: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_If(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-If: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_IF;

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.if_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.if_->exp2_;
    Exp *e3 = cncl_ob->exp_->u.if_->exp3_;
    Box *tau = getRootBox(cncl_ob->box_);

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = gamma;
    asmp_ob->cncl_->exp_ = e1;
    asmp_ob->cncl_->box_ = (Box *)malloc(sizeof(Box));
    asmp_ob->cncl_->box_->box_type = ROOT;
    asmp_ob->cncl_->box_->u.type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->box_->u.type_->type_type = BOOLT;
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = gamma;
    asmp_ob->next->cncl_->exp_ = e2;
    asmp_ob->next->cncl_->box_ = tau;
    asmp_ob->next->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->next->cncl_->env_ = gamma;
    asmp_ob->next->next->cncl_->exp_ = e3;
    asmp_ob->next->next->cncl_->box_ = tau;
    derivation(asmp_ob->cncl_,d+1);
    derivation(asmp_ob->next->cncl_,d+1);
    derivation(asmp_ob->next->next->cncl_,d+1);
    asmp_ob->next->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

#ifdef DBG_DRV
    tree(d);
    printf("T-If: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_Let(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Let: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_LET;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.let_->x;
    Exp *e1 = cncl_ob->exp_->u.let_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.let_->exp2_;
    Box *tau2 = getRootBox(cncl_ob->box_);
    Box *tau1 = (Box *)malloc(sizeof(Box));
    tau1->box_type = ROOT;
    tau1->u.type_ = (Type *)malloc(sizeof(Type));
    tau1->u.type_->type_type = TBD;
    tau1->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
    tau1->u.type_->u.tbd_->n = kdkd();

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = gamma;
    asmp_ob->cncl_->exp_ = e1;
    asmp_ob->cncl_->box_ = tau1;
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->next->cncl_->env_->prev = gamma;
    asmp_ob->next->cncl_->env_->var_ = x;
    asmp_ob->next->cncl_->env_->box_ = tau1;
    asmp_ob->next->cncl_->exp_ = e2;
    asmp_ob->next->cncl_->box_ = tau2;
    derivation(asmp_ob->cncl_,d+1);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

#ifdef DBG_DRV
    tree(d);
    printf("T-Let: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_Fun(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Fun: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_FUN;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.fun_->x;
    Exp *e = cncl_ob->exp_->u.fun_->exp_;
    Box *tau1, *tau2;

    Type *tmp = getRootBox(cncl_ob->box_)->u.type_;
    if(tmp->type_type==TBD){
        tau1 = (Box *)malloc(sizeof(Box));
        tau1->box_type = ROOT;
        tau1->u.type_ = (Type *)malloc(sizeof(Type));
        tau1->u.type_->type_type = TBD;
        tau1->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
        tau1->u.type_->u.tbd_->n = kdkd();
        tau2 = (Box *)malloc(sizeof(Box));
        tau2->box_type = ROOT;
        tau2->u.type_ = (Type *)malloc(sizeof(Type));
        tau2->u.type_->type_type = TBD;
        tau2->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
        tau2->u.type_->u.tbd_->n = kdkd();
        tmp->type_type = FUNT;
        tmp->u.funt_ = (Funt *)malloc(sizeof(Funt));
        tmp->u.funt_->box1_ = tau1;
        tmp->u.funt_->box2_ = tau2;
    }else if(tmp->type_type==FUNT){
        tau1 = getRootBox(tmp->u.funt_->box1_);
        tau2 = getRootBox(tmp->u.funt_->box2_);
    }else{
        error("error8");
    }

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->cncl_->env_->prev = gamma;
    asmp_ob->cncl_->env_->var_ = x;
    asmp_ob->cncl_->env_->box_ = tau1;
    asmp_ob->cncl_->exp_ = e;
    asmp_ob->cncl_->box_ = tau2;
    derivation(asmp_ob->cncl_,d+1);
    asmp_ob->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

#ifdef DBG_DRV
    tree(d);
    printf("T-Fun: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_App(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-App: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_APP;

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.app_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.app_->exp2_;
    Box *tau1 = (Box *)malloc(sizeof(Box));
    tau1->box_type = ROOT;
    tau1->u.type_ = (Type *)malloc(sizeof(Type));
    tau1->u.type_->type_type = TBD;
    tau1->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
    tau1->u.type_->u.tbd_->n = kdkd();
    Box *tau2 = getRootBox(cncl_ob->box_);

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = gamma;
    asmp_ob->cncl_->exp_ = e1;
    asmp_ob->cncl_->box_ = (Box *)malloc(sizeof(Box));
    asmp_ob->cncl_->box_->box_type = ROOT;
    asmp_ob->cncl_->box_->u.type_ = (Type *)malloc(sizeof(Type));
    asmp_ob->cncl_->box_->u.type_->type_type = FUNT;
    asmp_ob->cncl_->box_->u.type_->u.funt_ = (Funt *)malloc(sizeof(Funt));
    asmp_ob->cncl_->box_->u.type_->u.funt_->box1_ = tau1;
    asmp_ob->cncl_->box_->u.type_->u.funt_->box2_ = tau2;
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = gamma;
    asmp_ob->next->cncl_->exp_ = e2;
    asmp_ob->next->cncl_->box_ = tau1;
    derivation(asmp_ob->cncl_,d+1);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

#ifdef DBG_DRV
    tree(d);
    printf("T-App: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_LetRec(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-LetRec: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_LETREC;

    Env *gamma = cncl_ob->env_;
    Var *x = cncl_ob->exp_->u.letrec_->x;
    Var *y = cncl_ob->exp_->u.letrec_->y;
    Exp *e1 = cncl_ob->exp_->u.letrec_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.letrec_->exp2_;
    Box *tau = getRootBox(cncl_ob->box_);
    Box *tau1 = (Box *)malloc(sizeof(Box));
    tau1->box_type = ROOT;
    tau1->u.type_ = (Type *)malloc(sizeof(Type));
    tau1->u.type_->type_type = TBD;
    tau1->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
    tau1->u.type_->u.tbd_->n = kdkd();
    Box *tau2 = (Box *)malloc(sizeof(Box));
    tau2->box_type = ROOT;
    tau2->u.type_ = (Type *)malloc(sizeof(Type));
    tau2->u.type_->type_type = TBD;
    tau2->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
    tau2->u.type_->u.tbd_->n = kdkd();
    Box *tau1_tau2 = (Box *)malloc(sizeof(Box));
    tau1_tau2->box_type = ROOT;
    tau1_tau2->u.type_ = (Type *)malloc(sizeof(Type));
    tau1_tau2->u.type_->type_type = FUNT;
    tau1_tau2->u.type_->u.funt_ = (Funt *)malloc(sizeof(Funt));
    tau1_tau2->u.type_->u.funt_->box1_ = tau1;
    tau1_tau2->u.type_->u.funt_->box2_ = tau2;

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->cncl_->env_->prev = (Env *)malloc(sizeof(Env));
    asmp_ob->cncl_->env_->prev->prev = gamma;
    asmp_ob->cncl_->env_->prev->var_ = x;
    asmp_ob->cncl_->env_->prev->box_ = tau1_tau2;
    asmp_ob->cncl_->env_->var_ = y;
    asmp_ob->cncl_->env_->box_ = tau1;
    asmp_ob->cncl_->exp_ = e1;
    asmp_ob->cncl_->box_ = tau2;
//reverse start
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->next->cncl_->env_->prev = gamma;
    asmp_ob->next->cncl_->env_->var_ = x;
    asmp_ob->next->cncl_->env_->box_ = tau1_tau2;
    asmp_ob->next->cncl_->exp_ = e2;
    asmp_ob->next->cncl_->box_ = tau;
    derivation(asmp_ob->cncl_,d+1);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

#ifdef DBG_DRV
    tree(d);
    printf("T-LetRec: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}



void T_Nil(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Nil: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_NIL;
    cncl_ob->asmp_ = NULL;
    Box *tmp = getRootBox(cncl_ob->box_);
    if(tmp->u.type_->type_type==TBD){
        tmp->u.type_->type_type = LISTT;
        tmp->u.type_->u.listt_ = (Listt *)malloc(sizeof(Listt));
        tmp->u.type_->u.listt_->box_ = (Box *)malloc(sizeof(Box));
        tmp->u.type_->u.listt_->box_->box_type = ROOT;
        tmp->u.type_->u.listt_->box_->u.type_ = (Type *)malloc(sizeof(Type));
        tmp->u.type_->u.listt_->box_->u.type_->type_type = TBD;
        tmp->u.type_->u.listt_->box_->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
        tmp->u.type_->u.listt_->box_->u.type_->u.tbd_->n = kdkd();
        tmp->u.type_->u.listt_->box_->u.type_->type_type = TBD;
    }else if(getRootBox(cncl_ob->box_)->u.type_->type_type!=LISTT){
        error("Nil is not list.");
    }

#ifdef DBG_DRV
    tree(d);
    printf("T-Nil: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_Cons(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Cons: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_CONS;

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.cons_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.cons_->exp2_;
    Box *tau, *taulist;

    taulist = getRootBox(cncl_ob->box_);
    Type *tmp = taulist->u.type_;
    if(tmp->type_type==TBD){
        tau = (Box *)malloc(sizeof(Box));
        tau->box_type = ROOT;
        tau->u.type_ = (Type *)malloc(sizeof(Type));
        tau->u.type_->type_type = TBD;
        tau->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
        tau->u.type_->u.tbd_->n = kdkd();
        tmp->type_type = LISTT;
        tmp->u.listt_ = (Listt *)malloc(sizeof(Listt));
        tmp->u.listt_->box_ = tau;
    }else if(tmp->type_type==LISTT){
        tau = tmp->u.listt_->box_;
    }else{
        error("Cons is not list.");
    }

    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = gamma;
    asmp_ob->cncl_->exp_ = e1;
    asmp_ob->cncl_->box_ = tau;
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = gamma;
    asmp_ob->next->cncl_->exp_ = e2;
    asmp_ob->next->cncl_->box_ = taulist;
    derivation(asmp_ob->cncl_,d+1);
    derivation(asmp_ob->next->cncl_,d+1);
    asmp_ob->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

#ifdef DBG_DRV
    tree(d);
    printf("T-Cons: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    return;
}

void T_Match(Cncl *cncl_ob, int d){
#ifdef DBG_DRV
    tree(d);
    printf("T-Match: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

    cncl_ob->rule_type = T_MATCH;

    Env *gamma = cncl_ob->env_;
    Exp *e1 = cncl_ob->exp_->u.match_->exp1_;
    Exp *e2 = cncl_ob->exp_->u.match_->exp2_;
    Var *x = cncl_ob->exp_->u.match_->x;
    Var *y = cncl_ob->exp_->u.match_->y;
    Exp *e3 = cncl_ob->exp_->u.match_->exp3_;
    Box *tau = getRootBox(cncl_ob->box_);

    Box *tau1 = (Box *)malloc(sizeof(Box));
    tau1->box_type = ROOT;
    tau1->u.type_ = (Type *)malloc(sizeof(Type));
    tau1->u.type_->type_type = TBD;
    tau1->u.type_->u.tbd_ = (Tbd *)malloc(sizeof(Tbd));
    tau1->u.type_->u.tbd_->n = kdkd();
    Box *tau1list = (Box *)malloc(sizeof(Box));
    tau1list->box_type = ROOT;
    tau1list->u.type_ = (Type *)malloc(sizeof(Type));
    tau1list->u.type_->type_type = LISTT;
    tau1list->u.type_->u.listt_ = (Listt *)malloc(sizeof(Listt));
    tau1list->u.type_->u.listt_->box_ = tau1;

    //set asmp 1
    Asmp *asmp_ob = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->cncl_->env_ = gamma;
    asmp_ob->cncl_->exp_ = e1;
    asmp_ob->cncl_->box_ = tau1list;
    //set asmp 2
    asmp_ob->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->cncl_->env_ = gamma;
    asmp_ob->next->cncl_->exp_ = e2;
    asmp_ob->next->cncl_->box_ = tau;
    //set asmp 3
    asmp_ob->next->next = (Asmp *)malloc(sizeof(Asmp));
    asmp_ob->next->next->cncl_ = (Cncl *)malloc(sizeof(Cncl));
    asmp_ob->next->next->cncl_->env_ = (Env *)malloc(sizeof(Env));
    asmp_ob->next->next->cncl_->env_->prev = (Env *)malloc(sizeof(Env));
    asmp_ob->next->next->cncl_->env_->prev->prev = gamma;
    asmp_ob->next->next->cncl_->env_->prev->var_ = x;
    asmp_ob->next->next->cncl_->env_->prev->box_ = tau1;
    asmp_ob->next->next->cncl_->env_->var_ = y;
    asmp_ob->next->next->cncl_->env_->box_ = tau1list;
    asmp_ob->next->next->cncl_->exp_ = e3;
    asmp_ob->next->next->cncl_->box_ = tau;
    derivation(asmp_ob->cncl_,d+1);
    derivation(asmp_ob->next->cncl_,d+1);
    derivation(asmp_ob->next->next->cncl_,d+1);
    asmp_ob->next->next->next = NULL;
    cncl_ob->asmp_ = asmp_ob;

#ifdef DBG_DRV
    tree(d);
    printf("T-Match: ");
    writeEnv(cncl_ob->env_);
    printf(" |- ");
    writeExp(cncl_ob->exp_);
    printf(" : ");
    writeBox(cncl_ob->box_);
    printf("\n");
#endif

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
