typedef enum{//rule type
    E_INT,
    E_BOOL,
    E_IFT,
    E_IFF,
    E_PLUS,
    E_MINUS,
    E_TIMES,
    E_LT,
    E_VAR,
    E_LET,
    E_FUN,
    E_APP,
    E_LETREC,
    E_APPREC,
    B_PLUS,
    B_MINUS,
    B_TIMES,
    B_LT,
    E_NIL,
    E_CONS,
    E_MATCHNIL,
    E_MATCHCONS
}RuleType;

typedef enum{ //cncl type
    INFR,
    EVAL
}CnclType;

typedef enum{ //val type
    INT_,
    BOOL_,
    CLSR,
    CLSRREC,
    NIL_,
    CONS_
}ValType;

typedef enum{ // exp type
    INT,
    BOOL,
    VAR,
    OP,
    IF,
    LET,
    FUN,
    APP,
    LETREC,
    NIL,
    CONS,
    MATCH,

}ExpType;

typedef enum{ // infr type
    PLUS,
    MINUS,
    TIMES,
    LT
}InfrOpType;


struct Int_;
struct Bool_;
struct Clsr_;
struct ClsrRec_;
struct Consv_;

struct Env_;

struct Val_;
struct Var_;
struct Op_;
struct If_;
struct Let_;
struct Fun_;
struct App_;
struct LetRec_;
struct Conse_;
struct Match_;

struct Exp_;

struct Asmp_;

struct Infr_;
struct Eval_;

struct Cncl_;


typedef struct Int_{
    int i;
}Int;


typedef struct Bool_{
    int b;
}Bool;


typedef struct Clsr_{
    struct Env_ *env_;
    struct Var_ *arg;
    struct Exp_ *exp_;
}Clsr;

typedef struct ClsrRec_{
    struct Env_ *env_;
    struct Var_ *fun;
    struct Var_ *arg;
    struct Exp_ *exp_;
}ClsrRec;

typedef struct Consv_{
    struct Val_ *val1_;
    struct Val_ *val2_;
}Consv;

typedef struct Env_{
    struct Var_ *var_;
    struct Val_ *val_;
    struct Env_ *prev;
}Env;

typedef struct Val_{
    ValType val_type;
    union{
        struct Int_ *int_;
        struct Bool_ *bool_;
        struct Clsr_ *clsr_;
        struct ClsrRec_ *clsrrec_;
        struct Consv_ *consv_;
    }u;
}Val;

typedef struct Var_{
    char *var_name;
}Var;

typedef struct Op_{
    InfrOpType op_type;
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}Op;

typedef struct If_{
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
    struct Exp_ *exp3_;
}If;

typedef struct Let_{
    struct Var_ *var_;
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}Let;

typedef struct Fun_{
    struct Var_ *arg;
    struct Exp_ *exp_;
}Fun;

typedef struct App_{
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}App;

typedef struct LetRec_{
    struct Var_ *fun;
    struct Var_ *arg;
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}LetRec;

typedef struct Conse_{
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}Conse;

typedef struct Match_{
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
    struct Var_ *x;
    struct Var_ *y;
    struct Exp_ *exp3_;
}Match;

typedef struct Exp_{
    ExpType exp_type;
    union{
        struct Int_ *int_;
        struct Bool_ *bool_;
        struct Var_ *var_;
        struct Op_ *op_;
        struct If_ *if_;
        struct Let_ *let_;
        struct Fun_ *fun_;
        struct App_ *app_;
        struct LetRec_ *letrec_;
        struct Conse_ *conse_;
        struct Match_ *match_;
    }u;
}Exp;

typedef struct Asmp_{
    struct Cncl_ *cncl_;
    struct Asmp_ *next;
}Asmp;

typedef struct Infr_{
    InfrOpType infr_type;
    int int1;
    int int2;
    struct Val_ *val_;
}Infr;

typedef struct Eval_{
    struct Env_ *env_;
    struct Exp_ *exp_;
    struct Val_ *val_;
}Eval;

typedef struct Cncl_{
    CnclType cncl_type;
    RuleType rule_type;
    struct Asmp_ *asmp_;
    union{
        Infr *infr_;
        Eval *eval_;
    }u;
}Cncl;
