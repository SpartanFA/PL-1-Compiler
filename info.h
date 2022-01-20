
//Benjamin Werner 4705456

#define MAX_SYMBOL_TABLE_SIZE 1000
#define CODE_SIZE 1000
#define MAX_STACK_HEIGHT 1000
#define MAX_LEXEME_LIST 1000


typedef enum {
    nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    multsym = 6,  slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
    gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
    readsym = 32, elsesym = 33
}token_type;

struct token{
    token_type tokenID;
    int numberValue;
    char name[11];
};

extern struct token lexemeList[MAX_LEXEME_LIST];
extern int curLexemeListIndex;

//@TODO implement this to vm.c
 typedef struct {
     int op;
     int l;
     int m;
 } instruction;

extern instruction code[CODE_SIZE];

void compile();
void lex();
void parse();
void vm(int vmflag);
