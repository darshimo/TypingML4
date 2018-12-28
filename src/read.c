#include "param.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
void ind(int d);
#endif


void error(char *);

Funt *readFunt(char *, char *);
Listt *readListt(char *);
Env *readEnv(char *);
Box *readBox(char *);
Type *readType(char *);
Int *readInt(char *);
Bool *readBool(char *);
Var *readVar(char *);
If *readIf(char *);
Op *readOp(char *);
Let *readLet(char *);
Fun *readFun(char *);
App *readApp(char *);
LetRec *readLetRec(char *);
Cons *readCons(char *, char *);
Match *readMatch(char *);
Exp *readExp(char *);
Cncl *readCncl(char *);


Funt *readFunt(char *str1, char *str2){
#ifdef DEBUG
    printf("funtt : %s -> %s\n",str1,str2);
#endif
    Funt *ob = (Funt *)malloc(sizeof(Funt));
    ob->box1_ = readBox(str1);
    ob->box2_ = readBox(str2);
    return ob;
}

Listt *readListt(char *str){
#ifdef DEBUG
    printf("listt : %s\n",str);
#endif
    Listt *ob = (Listt *)malloc(sizeof(Listt));
    char *tmp1 = str;
    char *tmp2 = NULL;
    while(*tmp1!='\0'){
        tmp2 = tmp1;
        tmp1+=strcspn(tmp1," ");
        tmp1+=strspn(tmp1," ");
    }
    if(strncmp(tmp2,"list",4)==0){
        *tmp2 = '\0';
        ob->box_ = readBox(str);
    }else{
        error("invalid list-type.");
    }
    return ob;
}

Env *readEnv(char *str){
    if(*str=='\0')return NULL;

    Env *ob = (Env *)malloc(sizeof(Env));

    Env *env_tmp = NULL;

    char *str1, *str2;

    str1 = strrchr(str,',');

    if(str1==NULL){
        str1 = str;
    }else{
        *str1 = '\0';
        str1++;
        str1+=strspn(str1," ");
        env_tmp = readEnv(str);
    }
    str2 = strchr(str1,':');
    *str2 = '\0';
    str2++;
    str2+=strspn(str2," ");

#ifdef DEBUG
    printf("env : %s,%s\n",str1,str2);
#endif

    ob->var_ = readVar(str1);
    ob->box_ = readBox(str2);
    ob->prev = env_tmp;

    return ob;
}

Box *readBox(char *str){
    Box *ob = (Box *)malloc(sizeof(Box));

    ob->box_type = ROOT;
    ob->u.type_ = readType(str);

    return ob;
}

Type *readType(char* str){
    char *tmp = str;
    int count = 0;
    if(*str=='('){
        tmp++;
        while(1){
            tmp += strcspn(tmp,"()");
            if(*tmp=='('){
                count++;
            }else if(*tmp==')'){
                if(count==0)break;
                count--;
            }else{
                break;
            }
            tmp++;
        }
        char *tmp2 = tmp+1;
        if(*(tmp2+=strspn(tmp2," "))=='\0'){
            *tmp = '\0';
            str++;
            str+=strspn(str," ");
            return readType(str);
        };
    }
#ifdef DEBUG
    //printf("type : %s\n",str);
#endif
    Type *ob = (Type *)malloc(sizeof(Type));

    tmp += strcspn(tmp," ");
    tmp += strspn(tmp," ");
    if(*tmp=='\0'){//when type is int or bool
        if(strncmp(str,"int",3)==0){
            ob->type_type = INTT;
#ifdef DEBUG
            printf("intt : %s\n",str);
#endif
        }else if(strncmp(str,"bool",3)==0){
            ob->type_type = BOOLT;
#ifdef DEBUG
            printf("boolt : %s\n",str);
#endif
        }else{
            error("invalid type.");
        }
    }else{
        tmp = str;
        count = 0;
        while(1){
            tmp += strcspn(tmp,"()-");
            if(*tmp=='('){
                count++;
            }else if(*tmp==')'){
                count--;
            }else if(*tmp=='-'){
                if(count==0)break;
            }else{
                break;
            }
            tmp++;
        }
        if(*tmp=='-'){//when type is fun
            *tmp = '\0';
            tmp+=2;
            tmp+=strspn(tmp," ");
            ob->type_type = FUNT;
            ob->u.funt_ = readFunt(str, tmp);
        }else{//when type is list
            ob->type_type = LISTT;
            ob->u.listt_ = readListt(str);
        }
    }

    return ob;
}

Int *readInt(char *str){
#ifdef DEBUG
    printf("int : %s\n",str);
#endif
    Int *ob = (Int *)malloc(sizeof(Int));
    ob->i = atoi(str);
    return ob;
}


Bool *readBool(char *str){
#ifdef DEBUG
    printf("bool : %s\n",str);
#endif
    Bool *ob = (Bool *)malloc(sizeof(Bool));
    if(str[0]=='t')ob->b = 1;
    else ob->b = 0;
    return ob;
}


Var *readVar(char *str){
#ifdef DEBUG
    printf("var : %s\n",str);
#endif
    Var *ob = (Var *)malloc(sizeof(Var));
    *(str+strcspn(str," "))='\0';
    ob->var_name = (char *)malloc(sizeof(char)*(strlen(str)+1));
    strcpy(ob->var_name,str);
    return ob;
}


If *readIf(char *str){
#ifdef DEBUG
    printf("if  : %s\n",str);
#endif
    If *ob = (If *)malloc(sizeof(If));

    int count = 0;
    char *str1,*str2,*str3;
    str+=strcspn(str," ");
    str+=strspn(str," ");
    str1 = str;
    while(*str!='\0'){
        if(strncmp(str-1," if ", 4)==0){
            count++;
        }else if(strncmp(str-1," then ",6)==0){
            if(count){
                count--;
            }else{
                *str = '\0';
                str++;
                break;
            }
        }
        str+=strcspn(str," ");
        str+=strspn(str," ");
    }
    str+=strcspn(str," ");
    str+=strspn(str," ");
    str2 = str;
    while(*str!='\0'){
        if(strncmp(str-1," if ",4)==0){
            count++;
        }else if(strncmp(str-1," else ",6)==0){
            if(count){
                count--;
            }else{
                *str = '\0';
                str++;
                break;
            }
        }
        str+=strcspn(str," ");
        str+=strspn(str," ");
    }
    str+=strcspn(str," ");
    str+=strspn(str," ");
    str3 = str;

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);
    ob->exp3_ = readExp(str3);

    return ob;
}


Op *readOp(char* str){
#ifdef DEBUG
    printf("op  : %s\n",str);
#endif
    Op *ob = (Op *)malloc(sizeof(Op));

    int count = 0;
    ob->op_type = TIMES;
    char *str1, *str2;
    str1 = str;
    str2 = NULL;
    while(1){
        str += strcspn(str,"(+-*<)il");
        if(str[0]=='('){
            count++;
        }else if(str[0]==')'){
            count--;
        }else if(count==0){
            if(strncmp(str-1," if ",4)*strncmp(str-1," let ",5)==0){
                break;
            }else if(strncmp(str-1," + ",3)==0){
                ob->op_type=PLUS;
                str2 = str;
            }else if(strncmp(str-1," - ",3)==0){
                if(str==str1){
                    str++;
                    continue;
                }
                ob->op_type=MINUS;
                str2 = str;
            }else if(strncmp(str-1," < ",3)==0){
                ob->op_type=LT;
                str2 = str;
                break;
            }else if(strncmp(str-1," * ",3)==0){
                if(ob->op_type==TIMES){
                    str2 = str;
                }
            }else if(*str=='\0'){
                break;
            }
        }
        str++;
    }

    if(str2==NULL)error("invalid.");

    *str2 = '\0';
    str2++;
    str2 += strspn(str2," ");
    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);

    return ob;
}


Let *readLet(char *str){
#ifdef DEBUG
    printf("let : %s\n",str);
#endif
    Let *ob = (Let *)malloc(sizeof(Let));

    char *str1, *str2;

    str += strcspn(str," ");
    str += strspn(str," ");
    str1 = str;
    str1 += strcspn(str1," ");
    str1 += strspn(str1," ");
    *str1 = '\0';
    str1++;
    str1 += strspn(str1," ");
    str2 = str1;

    int count = 0;
    while(1){
        str2+=strcspn(str2,"li");
        if(strncmp(str2-1," let ",5)==0){
            count++;
        }else if(strncmp(str2-1," in ",4)==0){
            if(count==0){
                break;
            }
            count--;
        }else if(*str2=='\0'){
            error("2mismatch let and in.");
        }
        str2++;
    }

    *str2 = '\0';
    str2+=2;
    str2 += strspn(str2," ");

    ob->x = readVar(str);
    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);

    return ob;
}


Fun *readFun(char *str){
#ifdef DEBUG
    printf("fun : %s\n",str);
#endif
    Fun *ob = (Fun *)malloc(sizeof(Fun));
    char *tmp;
    str += strcspn(str," ");
    str += strspn(str," ");
    tmp = str;
    str += strcspn(str," ");
    str += strspn(str," ");
    str += strcspn(str," ");
    str += strspn(str," ");
    *(tmp+strcspn(tmp," "))='\0';

    ob->x = (Var *)malloc(sizeof(Var));
    ob->x->var_name = (char *)malloc(sizeof(char)*(strlen(tmp)+1));
    strcpy(ob->x->var_name,tmp);
    ob->exp_ = readExp(str);

    return ob;
}


App *readApp(char *str){
#ifdef DEBUG
    printf("app : %s\n",str);
#endif
    App *ob = (App *)malloc(sizeof(App));
    char *str1 = str;
    char *str2 = NULL;
    int count;

    while(1){
        if(*str=='('){
            count = 0;
            str++;
            while(1){
                str+=strcspn(str,"()");
                if(*str=='('){
                    count++;
                }else if(*str==')'){
                    if(count==0){
                        break;
                    }
                    count--;
                }else{
                    error("3mismatch parentheses.");
                }
                str++;
            }
            str++;
        }else{
            str+=strcspn(str," ");
        }
        str+=strspn(str," ");
        if(*str=='\0'){
            break;
        }else{
            str2 = str;
        }
    }

    if(str2==NULL)error("sole app.");

    *(str2-1)='\0';

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);
    return ob;
}


LetRec *readLetRec(char *str){
#ifdef DEBUG
    printf("letrec : %s\n",str);
#endif
    LetRec *ob = (LetRec *)malloc(sizeof(LetRec));
    char *tmp1, *tmp2, *tmp3;
    str += strcspn(str," ");
    str += strspn(str," ");
    str += strcspn(str," ");
    str += strspn(str," ");
    tmp1 = str;
    str += strcspn(str," ");
    str += strspn(str," ");
    str += strcspn(str," ");
    str += strspn(str," ");
    str += strcspn(str," ");
    str += strspn(str," ");
    tmp2 = str;
    str += strcspn(str," ");
    str += strspn(str," ");
    str += strcspn(str," ");
    str += strspn(str," ");
    tmp3 = str;
    *(tmp1+strcspn(tmp1," "))='\0';
    *(tmp2+strcspn(tmp2," "))='\0';

    ob->x = (Var *)malloc(sizeof(Var));
    ob->x->var_name = (char *)malloc(sizeof(char)*(strlen(tmp1)+1));
    strcpy(ob->x->var_name,tmp1);
    ob->y = (Var *)malloc(sizeof(Var));
    ob->y->var_name = (char *)malloc(sizeof(char)*(strlen(tmp2)+1));
    strcpy(ob->y->var_name,tmp2);

    int count = 0;
    while(1){
        str+=strcspn(str,"li");
        if(strncmp(str-1," let ",5)==0){
            count++;
        }else if(strncmp(str-1," in ",4)==0){
            if(count==0){
                break;
            }
            count--;
        }else if(*str=='\0'){
            error("4mismatch let and in.");
        }
        str++;
    }
    *str = '\0';
    str+=2;
    str += strspn(str," ");

    ob->exp1_ = readExp(tmp3);
    ob->exp2_ = readExp(str);

    return ob;
}


Cons *readCons(char *str1, char *str2){
#ifdef DEBUG
    printf("cons : %s :: %s\n",str1,str2);
#endif
    Cons *ob = (Cons *)malloc(sizeof(Cons));

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);

    return ob;
}


Match *readMatch(char *str){
#ifdef DEBUG
    printf("match : %s\n",str);
#endif
    Match *ob = (Match *)malloc(sizeof(Match));

    char *str1, *str2, *str3, *str4, *str5;
    int i, count;

    str += strcspn(str," ");
    str += strspn(str," ");
    str1 = str;

    count = 0;
    while(1){
        str += strcspn(str,"mw");
        if(strncmp(str-1," match ",7)==0){
            count++;
        }else if(strncmp(str-1," with ",6)==0){
            if(count>0){
                count--;
            }else if(count==0){
                break;
            }else{
                error("count error.");
            }
        }else{
            error("mismatch of match&with.");
        }
    }

    *str='\0';
    str++;

    for(i=0;i<3;i++){
        str += strcspn(str," ");
        str += strspn(str," ");
    }
    str2 = str;

    count = 0;
    while(1){
        str += strcspn(str,"m|");
        if(strncmp(str-1," match ",7)==0){
            count++;
        }else if(strncmp(str-1," | ",3)==0){
            if(count>0){
                count--;
            }else if(count==0){
                break;
            }else{
                error("count error.");
            }
        }else{
            error("mismatch of match&with.");
        }
    }

    *str='\0';
    str++;

    str += strspn(str," ");
    str3 = str;
    str += strcspn(str," ");
    str += strspn(str," ");
    *str='\0';
    str+=2;

    str += strspn(str," ");
    str4 = str;
    str += strcspn(str," ");
    str += strspn(str," ");
    *str='\0';
    str+=2;

    str += strspn(str," ");
    str5 = str;

    ob->exp1_ = readExp(str1);
    ob->exp2_ = readExp(str2);
    ob->x = readVar(str3);
    ob->y = readVar(str4);
    ob->exp3_ = readExp(str5);

    return ob;
}


Exp *readExp(char* str){

    if(str[0]=='('){
        int count = 0;
        char *tmp1, *tmp2;
        tmp1 = str+1;
        while(1){
            tmp1+=strcspn(tmp1,"()");
            if(*tmp1=='('){
                count++;
            }else if(*tmp1==')'){
                if(count==0){
                    break;
                }
                count--;
            }else{
                error("5mismatch parentheses.");
            }
            tmp1++;
        }
        if(*tmp1=='\0')error("6mismatch parentheses.");
        tmp2=tmp1 + 1;
        if(*(tmp2+strspn(tmp2," "))=='\0'){
            str++;
            *tmp1 = '\0';
            return readExp(str);
        }
    }

#ifdef DEBUG
    //printf("exp : %s\n",str);
#endif

    Exp *ob = (Exp *)malloc(sizeof(Exp));

    if(strncmp(str,"[]",2)==0){
        char *tmp = str+2;
        tmp+=strspn(tmp," ");
        if(*tmp!='\0'){
            error("not nil");
        }
        ob->exp_type = NIL;
    }else if(strncmp(str,"let rec ",8)==0){//when exp is letrec
        ob->exp_type = LETREC;
        ob->u.letrec_ = readLetRec(str);
    }else if(strncmp(str,"let ",4)==0){//when exp is let
        ob->exp_type = LET;
        ob->u.let_ = readLet(str);
    }else if(strncmp(str,"fun ",4)==0){//when exp is fun
        ob->exp_type = FUN;
        ob->u.fun_ = readFun(str);
    }else if(strncmp(str,"if ",3)==0){//when exp is if
        ob->exp_type = IF;
        ob->u.if_ = readIf(str);
    }else if(strncmp(str,"match ",6)==0){//when exp is match
        ob->exp_type = MATCH;
        ob->u.match_ = readMatch(str);
    }else{

        char *tmp;
        tmp = str;
        tmp += strspn(tmp,"0123456789");
        tmp += strspn(tmp," ");
        if(*tmp=='\0'){//when exp is int
            ob->exp_type = INT;
            ob->u.int_ = readInt(str);
        }else{

            tmp = str;
            tmp += strcspn(tmp," ()+-*<:");
            tmp += strspn(tmp," ");
            if(*tmp=='\0'){//when exp is var or bool
                if(strncmp(str,"true",4)*strncmp(str,"false",5)==0){
                    ob->exp_type = BOOL;
                    ob->u.bool_ = readBool(str);
                }else{
                    ob->exp_type = VAR;
                    ob->u.var_ = readVar(str);
                }
            }else{

                tmp = str;
                int count = 0;
                while(1){
                    tmp+=strcspn(tmp,"()+-*<:");
                    if(*tmp=='('){
                        count++;
                    }else if(*tmp==')'){
                        count--;
                    }else if(*tmp!='\0'){
                        if(count==0)break;
                    }else{
                        break;
                    }
                    tmp++;
                }
                if(*tmp==':'){//when exp is cons
                    *tmp = '\0';
                    tmp+=2;
                    tmp+=strspn(tmp," ");
                    ob->exp_type = CONS;
                    ob->u.cons_ = readCons(str,tmp);
                }else if(*tmp=='\0'){//when exp is app
                    ob->exp_type = APP;
                    ob->u.app_ = readApp(str);
                }else{//when exp is op
                    ob->exp_type = OP;
                    ob->u.op_ = readOp(str);
                }
            }
        }
    }

    return ob;
}


Cncl *readCncl(char* str){
#ifdef DEBUG
    printf("cncl: %s\n",str);
#endif
    Cncl *ob = (Cncl *)malloc(sizeof(Cncl));

    char *str1, *str2, *str3;
    str1 = str;
    str2 = strstr(str1,"|-");
    *str2 = '\0';
    str2+=2;
    str2+=strspn(str2," ");
    str3 = strstr(str2," : ");
    *str3 = '\0';
    str3+=3;
    str3+=strspn(str3," ");

    ob->env_ = readEnv(str1);
    ob->exp_ = readExp(str2);
    ob->box_ = readBox(str3);

    return ob;
}
