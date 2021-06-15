#include "code-generator.h"

//push from register to stack
void pushFrom(int resgister) {
    cout<<"sub $30,$30,$4"<<endl;
    cout<<"sw $"<<resgister<<", 0($30)"<<endl;
}

//pop from stack to register
void popTo(int resgister) {
    cout<<"lw $"<<resgister<<", 0($30)"<<endl;
    cout<<"add $30,$30,$4"<<endl;
}

//return function for main
void returnF(){
    cout<<"add $30,$30,$4"<<endl;
    cout<<"add $30,$30,$4"<<endl;
    cout<<"add $30,$30,$4"<<endl;
    cout<<"jr $31"<<endl;
}

//main function for code generation
void produce(ParseTree *&root, map<string,string> &functions,
             map<string, string> &symtable ,map<string,int> &offset,int &wnum,int &inum){
    if(root->me == "main"){
        cout<<"lis $5"<<endl;
        cout<<".word init"<<endl;
        if (validate(root->children.at(3), symtable, functions)=="int") {
            pushFrom(2);
            cout<<"add $2,$0,$0"<<endl;
            pushFrom(31);
            cout<<"jalr $5"<<endl;
            popTo(31);
            popTo(2);
        } else {
            pushFrom(31);
            cout<<"jalr $5"<<endl;
            popTo(31);
        }
        
        cout<<"main: "<<endl;
        cout<<"sub $29,$30,$4"<<endl;
        pushFrom(1);
        pushFrom(2);
        produce(root->children.at(8),functions,symtable,offset,wnum,inum);
        produce(root->children.at(9),functions,symtable,offset,wnum,inum);
        produce(root->children.at(11),functions,symtable,offset,wnum,inum);
        returnF();
    } else if(root->me == "expr"){
        string child = formchild(root);
        if(child == "expr term"){
            produce(root->children.at(0),functions,symtable,offset,wnum,inum);
        } else if(child == "expr expr PLUS term"){
            string type1 = validate(root->children.at(0), symtable, functions);
            string type2 = validate(root->children.at(2), symtable, functions);
            if(type1 == "int" && type2 == "int"){
                produce(root->children.at(0), functions,symtable,offset,wnum,inum);
                pushFrom(3);
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                popTo(5);
                cout<<"add $3,$5,$3"<<endl;
                cout<<endl;
            } else if(type1 == "int*" && type2 == "int"){
                produce(root->children.at(0), functions,symtable,offset,wnum,inum);
                pushFrom(3);
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                cout<<"mult $3,$4"<<endl;
                cout<<"mflo $3"<<endl;
                popTo(5);
                cout<<"add $3,$5,$3"<<endl;
                cout<<endl;
            } else if(type1 == "int" && type2 == "int*"){
                produce(root->children.at(0), functions,symtable,offset,wnum,inum);
                cout<<"mult $3,$4"<<endl;
                cout<<"mflo $3"<<endl;
                pushFrom(3);
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                popTo(5);
                cout<<"add $3,$5,$3"<<endl;
                cout<<endl;
            }
        } else if (child == "expr expr MINUS term") {
            string type1 = validate(root->children.at(0), symtable, functions);
            string type2 = validate(root->children.at(2), symtable, functions);
            if(type1 == "int" && type2 == "int"){
                produce(root->children.at(0), functions,symtable,offset,wnum,inum);
                pushFrom(3);
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                popTo(5);
                cout<<"sub $3,$5,$3"<<endl;
                cout<<endl;
            } else if(type1 == "int*" && type2 == "int"){
                produce(root->children.at(0), functions,symtable,offset,wnum,inum);
                pushFrom(3);
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                cout<<"mult $3,$4"<<endl;
                cout<<"mflo $3"<<endl;
                popTo(5);
                cout<<"sub $3,$5,$3"<<endl;
                cout<<endl;
            } else if(type1 == "int*" && type2 == "int*"){
                produce(root->children.at(0), functions,symtable,offset,wnum,inum);
                pushFrom(3);
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                popTo(5);
                cout<<"sub $3,$5,$3"<<endl;
                cout<<"div $3,$4"<<endl;
                cout<<"mflo $3"<<endl;
                cout<<endl;
            }
        }
    } else if(root->me == "term"){
        string child = formchild(root);
        if(child == "term factor"){
            produce(root->children.at(0), functions,symtable,offset,wnum,inum);
        } else if(child == "term term STAR factor"){
            produce(root->children.at(0), functions,symtable,offset,wnum,inum);
            pushFrom(3);
            produce(root->children.at(2), functions,symtable,offset,wnum,inum);
            popTo(5);
            cout<<"mult $5,$3"<<endl;
            cout<<"mflo $3"<<endl;
            cout<<endl;
        } else if(child == "term term SLASH factor"){
            produce(root->children.at(0), functions,symtable,offset,wnum,inum);
            pushFrom(3);
            produce(root->children.at(2), functions,symtable,offset,wnum,inum);
            popTo(5);
            cout<<"div $5,$3"<<endl;
            cout<<"mflo $3"<<endl;
            cout<<endl;
        } else if(child == "term term PCT factor"){
            produce(root->children.at(0), functions,symtable,offset,wnum,inum);
            pushFrom(3);
            produce(root->children.at(2), functions,symtable,offset,wnum,inum);
            popTo(5);
            cout<<"div $5,$3"<<endl;
            cout<<"mfhi $3"<<endl;
            cout<<endl;
        }
    } else if(root->me == "factor"){
        string child = formchild(root);
        if(child == "factor ID"){
            cout<<"lw $3, "<<offset[root->children.at(0)->children.at(0)->me]<<"($29)"<<endl;
            cout<<endl;
        } else if(child == "factor LPAREN expr RPAREN"){
            produce(root->children.at(1), functions,symtable,offset,wnum,inum);
        } else if(child == "factor NUM"){
            cout<<"lis $3"<<endl;
            cout<<".word "<<root->children.at(0)->children.at(0)->me<<endl;
            cout<<endl;
        } else if(child == "factor NULL"){
            cout<<"add $3,$0,$11"<<endl;
            cout<<endl;
        } else if(child == "factor STAR factor"){
            produce(root->children.at(1), functions, symtable, offset, wnum, inum);
            cout<<"lw $3,0($3)"<<endl;
            cout<<endl;
        } else if(child == "factor NEW INT LBRACK expr RBRACK"){
            produce(root->children.at(3), functions, symtable, offset, wnum, inum);
            pushFrom(1);
            cout<<"add $1,$3,$0"<<endl;
            cout<<"lis $5"<<endl;
            cout<<".word new"<<endl;
            pushFrom(31);
            cout<<"jalr $5"<<endl;
            popTo(31);
            popTo(1);
            cout<<"bne $3,$0,1"<<endl;
            cout<<"add $3,$11,$0"<<endl;
        } else if(child == "factor AMP lvalue"){
            ParseTree *lvalue = root->children.at(1);
            string child = formchild(lvalue);
            if(child == "lvalue ID"){
                int off = offset[lvalue->children.at(0)->children.at(0)->me];
                cout<<"lis $5"<<endl;
                cout<<".word "<<off<<endl;
                cout<<"add $3,$5,$29"<<endl;
                cout<<endl;
            } else if(child == "lvalue STAR factor"){
                produce(lvalue->children.at(1), functions, symtable, offset, wnum, inum);
                cout<<endl;
            } else if(child == "lvalue LPAREN lvalue RPAREN"){
                root->children.at(1) = lvalue->children.at(1);
                produce(root, functions, symtable, offset, wnum, inum);
            }
        } else if(child == "factor ID LPAREN RPAREN"){
            pushFrom(29);
            pushFrom(31);
            cout<<"lis $5"<<endl;
            cout<<".word F"<<root->children.at(0)->children.at(0)->me<<endl;
            cout<<"jalr $5"<<endl;
            popTo(31);
            popTo(29);
        } else if(child == "factor ID LPAREN arglist RPAREN"){
            pushFrom(29);
            pushFrom(31);
            cout<<"add $6,$30,$0"<<endl;
            produce(root->children.at(2), functions, symtable, offset, wnum, inum);
            cout<<"lis $5"<<endl;
            cout<<".word F"<<root->children.at(0)->children.at(0)->me<<endl;
            cout<<"jalr $5"<<endl;
            cout<<"add $30,$6,$0"<<endl;
            popTo(31);
            popTo(29);
        }
    } else if(root->me == "statements"){
        if(root->children.size()>0){
            produce(root->children.at(0), functions,symtable,offset,wnum,inum);
            produce(root->children.at(1), functions,symtable,offset,wnum,inum);
        }
    } else if(root->me == "statement"){
        string child = formchild(root);
        if(child == "statement PRINTLN LPAREN expr RPAREN SEMI"){
            pushFrom(1);
            produce(root->children.at(2), functions,symtable,offset,wnum,inum);
            cout<<"add $1,$3,$0"<<endl;
            pushFrom(31);
            cout<<"jalr $10"<<endl;
            popTo(31);
            popTo(1);
            cout<<endl;
        } else if(child == "statement lvalue BECOMES expr SEMI"){
            ParseTree* lvalue = root->children.at(0);
            string child = formchild(lvalue);
            if(child == "lvalue ID"){
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                produce(root->children.at(0), functions,symtable,offset,wnum,inum);
            } else if(child == "lvalue STAR factor"){
                produce(root->children.at(2), functions,symtable,offset,wnum,inum);
                pushFrom(3);
                produce(lvalue->children.at(1), functions, symtable, offset, wnum, inum);
                popTo(5);
                cout<<"sw $5,0($3)"<<endl;
                cout<<endl;
            } else if(child == "lvalue LPAREN lvalue RPAREN"){
                root->children.at(0) = lvalue->children.at(1);
                produce(root, functions, symtable, offset, wnum, inum);
            }
        } else if(child == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
            int curwnum = wnum;
            wnum++;
            cout<<"whilestart"<<curwnum<<":"<<endl;
            produce(root->children.at(2), functions, symtable, offset, wnum, inum);
            cout<<"bne $3,$0,whileend"<<curwnum<<endl;
            produce(root->children.at(5), functions, symtable, offset, wnum, inum);
            cout<<"beq $0,$0,whilestart"<<curwnum<<endl;
            cout<<"whileend"<<curwnum<<":"<<endl;
        } else if(child ==
                  "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){
            int curinum = inum;
            inum++;
            produce(root->children.at(2), functions, symtable, offset, wnum, inum);
            cout<<"bne $3,$0,else"<<curinum<<endl;
            produce(root->children.at(5), functions, symtable, offset, wnum, inum);
            cout<<"beq $0,$0,endif"<<curinum<<endl;
            cout<<"else"<<curinum<<":"<<endl;
            produce(root->children.at(9), functions, symtable, offset, wnum, inum);
            cout<<"endif"<<curinum<<":"<<endl;
        } else if(child == "statement DELETE LBRACK RBRACK expr SEMI"){
            produce(root->children.at(3), functions, symtable, offset, wnum, inum);
            pushFrom(1);
            cout<<"beq $3,$11,skipDelete"<<endl;
            cout<<"add $1,$3,$0"<<endl;
            cout<<"lis $5"<<endl;
            cout<<".word delete"<<endl;
            pushFrom(31);
            cout<<"jalr $5"<<endl;
            popTo(31);
            popTo(1);
            cout<<"skipDelete:"<<endl;
        }
    } else if(root->me == "lvalue"){
        string child = formchild(root);
        if(child == "lvalue ID"){
            cout<<"sw $3,"<<offset[root->children.at(0)->children.at(0)->me]<<"($29)"<<endl;
            cout<<endl;
        } else if(child == "lvalue LPAREN lvalue RPAREN"){
            produce(root->children.at(1), functions,symtable,offset,wnum,inum);
        }
    } else if(root->me == "dcls"){
        string child = formchild(root);
        if (child == "dcls dcls dcl BECOMES NUM SEMI") {
            produce(root->children.at(0), functions,symtable,offset,wnum,inum);
            cout<<"lis $5"<<endl;
            cout<<".word "<<root->children.at(3)->children.at(0)->me<<endl;
            pushFrom(5);
            cout<<endl;
        } else if(child == "dcls dcls dcl BECOMES NULL SEMI"){
            produce(root->children.at(0), functions,symtable,offset,wnum,inum);
            cout<<"lis $5"<<endl;
            cout<<".word 0x1"<<endl;
            pushFrom(5);
            cout<<endl;
        }
    } else if(root->me == "test"){
        string type1 = validate(root->children.at(0), symtable, functions);
        string unsign = "";
        if(type1 == "int*") unsign = "u";
        string child = formchild(root);
        produce(root->children.at(0), functions, symtable, offset, wnum, inum);
        pushFrom(3);
        produce(root->children.at(2), functions, symtable, offset, wnum, inum);
        popTo(5);
        if(child == "test expr LT expr"){
            cout<<"slt"<<unsign<<" $5,$5,$3"<<endl;
            cout<<"add $3,$4,$0"<<endl;
            cout<<"beq $5,$0,1"<<endl;
            cout<<"add $3,$0,$0"<<endl;
        } else if(child == "test expr EQ expr"){
            cout<<"sub $5,$3,$5"<<endl;
            cout<<"add $3,$4,$0"<<endl;
            cout<<"bne $5,$0,1"<<endl;
            cout<<"add $3,$0,$0"<<endl;
        } else if(child == "test expr NE expr"){
            cout<<"sub $5,$3,$5"<<endl;
            cout<<"add $3,$4,$0"<<endl;
            cout<<"beq $5,$0,1"<<endl;
            cout<<"add $3,$0,$0"<<endl;
        } else if(child == "test expr LE expr"){
            pushFrom(3);
            pushFrom(5);
            popTo(3);
            popTo(5);
            cout<<"slt"<<unsign<<" $5,$5,$3"<<endl;
            cout<<"add $3,$4,$0"<<endl;
            cout<<"bne $5,$0,1"<<endl;
            cout<<"add $3,$0,$0"<<endl;
        } else if(child == "test expr GT expr"){
            pushFrom(3);
            pushFrom(5);
            popTo(3);
            popTo(5);
            cout<<"slt"<<unsign<<" $5,$5,$3"<<endl;
            cout<<"add $3,$4,$0"<<endl;
            cout<<"beq $5,$0,1"<<endl;
            cout<<"add $3,$0,$0"<<endl;
        } else if(child == "test expr GE expr"){
            cout<<"slt"<<unsign<<" $5,$5,$3"<<endl;
            cout<<"add $3,$4,$0"<<endl;
            cout<<"bne $5,$0,1"<<endl;
            cout<<"add $3,$0,$0"<<endl;
        }
        cout<<endl;
    } else if(root->me == "procedure"){
        cout<<"F"<<root->children.at(1)->children.at(0)->me<<":"<<endl;
        cout<<"sub $29, $30, $4"<<endl;
        produce(root->children.at(6), functions, symtable, offset, wnum, inum);
        for(int i = 1;i<8;i++){
            if(i!=3){
                pushFrom(i);
            }
        }
        produce(root->children.at(7), functions, symtable, offset, wnum, inum);
        produce(root->children.at(9), functions, symtable, offset, wnum, inum);
        for(int i = 7;i>0;i--){
            if(i!=3){
                popTo(i);
            }
        }
        cout<<"add $30, $29, $4"<<endl;
        cout<<"jr $31"<<endl;
    } else if(root->me == "arglist"){
        produce(root->children.at(0), functions, symtable, offset, wnum, inum);
        pushFrom(3);
        if(root->children.size()>1){
            produce(root->children.at(2), functions, symtable, offset, wnum, inum);
        }
    }
}
