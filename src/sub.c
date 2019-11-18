#include <stdio.h>
#include <stdlib.h>
#include "param.h"

#ifdef DBG_ALL
#define DBG_SB
#endif

int cmpVar(Var *, Var *);
Type *copyType(Type *);
Env *copyEnv(Env *);
Var *copyVar(Var *);
void writeBox(Box *);
void error(char *);

int kdkd()
{
    static int n = 1;
    return n++;
}

void ind(int d)
{
    int i;
    for (i = 0; i < d; i++)
    {
        printf("  ");
    }
    return;
}

void tree(int d)
{
    int i;
    for (i = 0; i < d; i++)
    {
        printf("\x1b[%dm", (i % 6) + 31);
        if (i < d - 1)
            printf("│ ");
        else
            printf("├─");
    }
    printf("\x1b[39m");
    return;
}

void writeRuleName(Cncl *cncl_ob)
{
    RuleType tmp = cncl_ob->rule_type;
    if (tmp == T_INT)
        printf("T-Int");
    else if (tmp == T_BOOL)
        printf("T-Bool");
    else if (tmp == T_IF)
        printf("T-If");
    else if (tmp == T_PLUS)
        printf("T-Plus");
    else if (tmp == T_MINUS)
        printf("T-Minus");
    else if (tmp == T_TIMES)
        printf("T-Times");
    else if (tmp == T_LT)
        printf("T-Lt");
    else if (tmp == T_VAR)
        printf("T-Var");
    else if (tmp == T_LET)
        printf("T-Let");
    else if (tmp == T_FUN)
        printf("T-Fun");
    else if (tmp == T_APP)
        printf("T-App");
    else if (tmp == T_LETREC)
        printf("T-LetRec");
    else if (tmp == T_NIL)
        printf("T-Nil");
    else if (tmp == T_CONS)
        printf("T-Cons");
    else
        printf("T-Match");
    return;
}

Box *getRootBox(Box *ob)
{
    if (ob->box_type == ROOT)
        return ob;
    return getRootBox(ob->u.prev);
}

Box *getEnvBox(Env *gamma, Var *x)
{
    if (gamma == NULL)
        error("var is not found.");
    if (cmpVar(gamma->var_, x) == 0)
        return gamma->box_;
    return getEnvBox(gamma->prev, x);
}

void integrateBox(Box *ob1, Box *ob2)
{
    ob1 = getRootBox(ob1);
    ob2 = getRootBox(ob2);
#ifdef DBG_SB
    printf("integrate: ");
    writeBox(ob1);
    printf(" : ");
    writeBox(ob2);
    printf(" :\n");
#endif
    if (ob1 == ob2)
    {
#ifdef DBG_SB
        printf("same pointer\n");
#endif
        return;
    }
    TypeType tmp1 = ob1->u.type_->type_type;
    TypeType tmp2 = ob2->u.type_->type_type;
    if (tmp1 == TBD)
    {
        free(ob1->u.type_->u.tbd_);
        free(ob1->u.type_);
        ob1->box_type = NODE;
        ob1->u.prev = ob2;
        return;
    }
    else if (tmp2 == TBD)
    {
        free(ob2->u.type_->u.tbd_);
        free(ob2->u.type_);
        ob2->box_type = NODE;
        ob2->u.prev = ob1;
        return;
    }
    else if (tmp1 != tmp2)
    {
        writeBox(ob1);
        printf("\n");
        writeBox(ob2);
        printf("\n");
        error("mismatch type in integration.");
    }
    else if (tmp1 == FUNT)
    {
        integrateBox(ob1->u.type_->u.funt_->box1_, ob2->u.type_->u.funt_->box1_);
        integrateBox(ob1->u.type_->u.funt_->box2_, ob2->u.type_->u.funt_->box2_);
    }
    else if (tmp1 == LISTT)
    {
        integrateBox(ob1->u.type_->u.listt_->box_, ob2->u.type_->u.listt_->box_);
    }
    else
    {
        free(ob1->u.type_);
        ob1->box_type = NODE;
        ob1->u.prev = ob2;
    }
    return;
}

void replaceTBD(Box *box_ob)
{
    Type *ob = getRootBox(box_ob)->u.type_;
    TypeType tmp = ob->type_type;
    if (tmp == TBD)
        ob->type_type = INTT;
    else if (tmp == FUNT)
    {
        replaceTBD(ob->u.funt_->box1_);
        replaceTBD(ob->u.funt_->box2_);
    }
    else if (tmp == LISTT)
    {
        replaceTBD(ob->u.listt_->box_);
    }
    return;
}

void replaceAll(Cncl *ob)
{
    Asmp *tmp1 = ob->asmp_;
    while (tmp1 != NULL)
    {
        replaceAll(tmp1->cncl_);
        tmp1 = tmp1->next;
    }
    Env *tmp2 = ob->env_;
    while (tmp2 != NULL)
    {
        replaceTBD(tmp2->box_);
        tmp2 = tmp2->prev;
    }
    replaceTBD(ob->box_);
    return;
}
