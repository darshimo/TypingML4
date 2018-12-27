typedef enum{//rule type
    T_INT,
    T_BOOL,
    T_IF,
    T_PLUS,
    T_MINUS,
    T_TIMES,
    T_LT,
    T_VAR,
    T_LET,
    T_FUN,
    T_APP,
    T_LETREC,
    T_NIL,
    T_CONS,
    T_MATCT
}RuleType;

typedef enum{ //type type
    TBD,
    INTT,
    BOOLT,
    FUNT,
    LISTT
}TypeType;

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
    MATCH
}ExpType;

typedef enum{ // infr type
    PLUS,
    MINUS,
    TIMES,
    LT
}OpType;


struct Funt_;
struct Listt_;

struct Env_;

struct Type_;

struct Int_;
struct Bool_;
struct Var_;
struct Op_;
struct If_;
struct Let_;
struct Fun_;
struct App_;
struct LetRec_;
struct Cons_;
struct Match_;

struct Exp_;

struct Asmp_;

struct Cncl_;


typedef struct Funt_{
    struct Type_ *type1_;
    struct Type_ *type2_;
}Funt;

typedef struct Listt_{
    struct Type_ *type_;
}Listt;

typedef struct Env_{
    struct Var_ *var_;
    struct Type_ *type_;
    struct Env_ *prev;
}Env;

typedef struct Type_{
    TypeType type_type;
    union{
        struct Funt_ *funt_;
        struct Listt_ *listt_;
    }u;
}Type;

typedef struct Int_{
    int i;
}Int;


typedef struct Bool_{
    int b;
}Bool;

typedef struct Var_{
    char *var_name;
}Var;

typedef struct Op_{
    OpType op_type;
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}Op;

typedef struct If_{
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
    struct Exp_ *exp3_;
}If;

typedef struct Let_{
    struct Var_ *x;
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}Let;

typedef struct Fun_{
    struct Var_ *x;
    struct Exp_ *exp_;
}Fun;

typedef struct App_{
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}App;

typedef struct LetRec_{
    struct Var_ *x;
    struct Var_ *y;
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}LetRec;

typedef struct Cons_{
    struct Exp_ *exp1_;
    struct Exp_ *exp2_;
}Cons;

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
        struct Cons_ *cons_;
        struct Match_ *match_;
    }u;
}Exp;

typedef struct Asmp_{
    struct Cncl_ *cncl_;
    struct Asmp_ *next;
}Asmp;

typedef struct Cncl_{
    RuleType rule_type;
    struct Asmp_ *asmp_;
    struct Env_ *env_;
    struct Exp_ *exp_;
    struct Type_ *type_;
}Cncl;
