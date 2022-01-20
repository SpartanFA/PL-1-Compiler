//Benjamin Werner UCFID 4705456

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "info.h"


//Reserved words array
const char* reservedWords[] = {
    "const", "var", "procedure", "call", "if", "then", "else", "while", "do", "read", "write", "odd", "begin", "end"
    };

const char *symRep[] = { "ERROR: UNREACHABLE SYMBOL", "nulsym", "identsym", "numbersym", "plussym", "minussym",
    "multsym",  "slashsym", "oddsym", "eqlsym", "neqsym", "lessym", "leqsym",
    "gtrsym", "geqsym", "lparentsym", "rparentsym", "commasym", "semicolonsym",
    "periodsym", "becomessym", "beginsym", "endsym", "ifsym", "thensym",
    "whilesym", "dosym", "callsym", "constsym", "varsym", "procsym", "writesym",
    "readsym" , "elsesym"
  };

//Special Symbols array
const char specialSymbols[] = {
    '+', '-', '*', '/', '(', ')', '=', ',' , '.', '<',
    '>',  ';' , ':', '{', '}'
    };

void charAlphabet();
void charDigit();
void charSpecialSymbol();
void print();
int getReservedWord(char *input, int size);
int getSpecialSymbol(int input);


//Initialize array of lexem and an index.
struct token lexemeList[MAX_LEXEME_LIST];
int curLexemeListIndex = 0;


//Declare file io variables.
//main input file
FILE *ifp;

//lexical output, used in parser
FILE *ofp;

//Full output
FILE *lexDebug;

/*
Varianbles--
i - reusable index variable
nextCharacterFlag - 1 if we are observing the next character, 0 - if we aren't
character - the ASCII value of the character we are observing
*/

int i, nextCharacterFlag, character;




void lex(void){

    curLexemeListIndex = 0;
    char fname[128] = "input.txt";

  //Try to the file directed in the Readme
    ifp = fopen(fname, "r");

    //If the file doesn't prompt for the name of an input file
    while(ifp == NULL){
        printf("\nEnter a valid .txt input file:\n");
        scanf("%s", fname);
        ifp = fopen(fname, "r");
    }

    ofp = fopen("lexoutput.txt", "w");
    lexDebug = fopen("lexdebug.txt", "w");
    ifp = fopen(fname,"r");

    fprintf(lexDebug,"Source Program:\n");

    while(character != EOF){
        fprintf(lexDebug, "%c", character);

        //Firstly ignore control values ie '\n' and ' ' so that we are only dealing with characters we care about
        if(iscntrl(character) || character == ' '){
            character = fgetc(ifp);
            nextCharacterFlag = 0;
            continue;
        }

        if(isalpha(character))
            charAlphabet();

        else if(isdigit(character))
            charDigit();

        else
            charSpecialSymbol();

        if(nextCharacterFlag == 0){
            character = fgetc(ifp);
        }

    }

    print();
    fclose(ofp);
    fclose(lexDebug);
    fclose(ifp);

}


//Return -1 if it is not a special symbol or the index of the specialSymbol.
int getSpecialSymbol(int input){
    int retval = -1;
    for(i=0 ; i < 15 ; i++){
        if(input == specialSymbols[i]){
                retval = i;
                break;
            }
        }

    return retval;
}

//Return -1 if it is not a reservedWord or whatever the index of the Reserved Word.
int getReservedWord(char *input, int size){
    int retval = -1;
    for(i=0 ; i < size ; i++){
        if(strcmp(input, reservedWords[i]) == 0){
                retval = i;
                break;
            }
        }

    return retval;
}

void charAlphabet(){

    char characterString[11];
    memset(characterString, 0, sizeof characterString);

    int strIndex = 0;
    characterString[strIndex] = character;
    strIndex = 1;

    nextCharacterFlag = 1;

    //Build the string and error out if it is too long.
    while(isalpha(character = fgetc(ifp)) || isdigit(character)){

        if(strIndex >= 11){
            printf("ERROR NAME TOO LONG.\n");
            break;
        }

        characterString[strIndex] = character;
        strIndex++;
        fprintf(lexDebug, "%c", character);
    }


    //If it is a reserved word, print out the correct tokentype
    switch(getReservedWord(characterString, 14)){

        //const
        case 0:
            strcpy(lexemeList[curLexemeListIndex].name, "const");
            lexemeList[curLexemeListIndex++].tokenID = constsym;
            break;

       //var
        case 1:
            strcpy(lexemeList[curLexemeListIndex].name, "var");
            lexemeList[curLexemeListIndex++].tokenID = varsym;
            break;

        //procedure
        case 2:
            strcpy(lexemeList[curLexemeListIndex].name, "procedure");
            lexemeList[curLexemeListIndex++].tokenID = procsym;
            break;

        //call
        case 3:
            strcpy(lexemeList[curLexemeListIndex].name, "call");
            lexemeList[curLexemeListIndex++].tokenID = callsym;
            break;

        //if
        case 4:
            strcpy(lexemeList[curLexemeListIndex].name, "if");
            lexemeList[curLexemeListIndex++].tokenID = ifsym;
            break;

        //then
        case 5:
            strcpy(lexemeList[curLexemeListIndex].name, "then");
            lexemeList[curLexemeListIndex++].tokenID = thensym;
            break;

        //else
        case 6:
            strcpy(lexemeList[curLexemeListIndex].name, "else");
            lexemeList[curLexemeListIndex++].tokenID = elsesym;
            break;

        //while
        case 7:
            strcpy(lexemeList[curLexemeListIndex].name, "while");
            lexemeList[curLexemeListIndex++].tokenID = whilesym;
            break;

        //do
        case 8:
            strcpy(lexemeList[curLexemeListIndex].name, "do");
            lexemeList[curLexemeListIndex++].tokenID = dosym;
            break;

        //read
        case 9:
            strcpy(lexemeList[curLexemeListIndex].name, "read");
            lexemeList[curLexemeListIndex++].tokenID = readsym;
            break;

        //write
        case 10:
            strcpy(lexemeList[curLexemeListIndex].name, "write");
            lexemeList[curLexemeListIndex++].tokenID = writesym;
            break;

        //odd
        case 11:
            strcpy(lexemeList[curLexemeListIndex].name, "odd");
            lexemeList[curLexemeListIndex++].tokenID = oddsym;
            break;

        //begin
        case 12:
            strcpy(lexemeList[curLexemeListIndex].name, "begin");
            lexemeList[curLexemeListIndex++].tokenID = beginsym;
            break;

        //end
        case 13:
            strcpy(lexemeList[curLexemeListIndex].name, "end");
            lexemeList[curLexemeListIndex++].tokenID = endsym;
            break;

        default:
            lexemeList[curLexemeListIndex].tokenID = identsym;
            strcpy(lexemeList[curLexemeListIndex].name, characterString);
            curLexemeListIndex++;
            break;
        }

}

void charDigit(){

    int fullNumber = character - '0';
    int digit;
    int numIndex = 1;

    nextCharacterFlag=1;


    //Makes sure the variable is not a letter
    if(isalpha(character)){
        printf("Error Variable does not start with letter\n");
    }

    //Build digits to make a number.
    while(isdigit(character = fgetc(ifp))){
        if( numIndex >= 5 ){
            printf("Error Number too long\n");
            break;
        }

        digit = character - '0';
        fullNumber = (10 * fullNumber) + digit;
        numIndex++;
        fprintf(lexDebug, "%c", character);
    }

    lexemeList[curLexemeListIndex].tokenID = numbersym;
    lexemeList[curLexemeListIndex].numberValue = fullNumber;
    curLexemeListIndex++;

}

void charSpecialSymbol(){

    nextCharacterFlag = 0;

    //If it is a special symbol, print out the correct tokentype
    switch(getSpecialSymbol(character)){

        // +
        case 0:
            strcpy(lexemeList[curLexemeListIndex].name, "+");
            lexemeList[curLexemeListIndex++].tokenID = plussym;
            break;

        // -
        case 1:
            strcpy(lexemeList[curLexemeListIndex].name, "-");
            lexemeList[curLexemeListIndex++].tokenID = minussym;
            break;

        // *
        case 2:
            strcpy(lexemeList[curLexemeListIndex].name, "*");
            lexemeList[curLexemeListIndex++].tokenID = multsym;
            break;

        // /
        case 3:

            character = fgetc(ifp);
            nextCharacterFlag = 1;

            if(character=='*'){
                int commentsFlag = 1;
                nextCharacterFlag = 0;
                character = fgetc(ifp);

                while(commentsFlag == 1){
                    if(character == '*'){
                        character = fgetc(ifp);
                        if(character == '/'){
                            commentsFlag = 0;
                        }
                    }

                    else{
                        character = fgetc(ifp);
                    }
                }

            }

            //If we aren't doing comments set to slash
            else{
                strcpy(lexemeList[curLexemeListIndex].name, "/");
                lexemeList[curLexemeListIndex++].tokenID = slashsym;
            }
            break;

        // (
        case 4:
            strcpy(lexemeList[curLexemeListIndex].name, "(");
            lexemeList[curLexemeListIndex++].tokenID = lparentsym;
            break;

        // )
        case 5:
            strcpy(lexemeList[curLexemeListIndex].name, ")");
            lexemeList[curLexemeListIndex++].tokenID = rparentsym;
            break;

        // =
        case 6:
            strcpy(lexemeList[curLexemeListIndex].name, "=");
            lexemeList[curLexemeListIndex++].tokenID = eqsym;
            break;

        // ,
        case 7:
            strcpy(lexemeList[curLexemeListIndex].name, ",");
            lexemeList[curLexemeListIndex++].tokenID = commasym;
            break;

        // .
        case 8:
            strcpy(lexemeList[curLexemeListIndex].name, ".");
            lexemeList[curLexemeListIndex++].tokenID = periodsym;
            break;

        // <
        case 9:
            character = fgetc(ifp);
            nextCharacterFlag=1;
            if(character == '>'){
                strcpy(lexemeList[curLexemeListIndex].name, "<>");
                lexemeList[curLexemeListIndex++].tokenID = neqsym;
                nextCharacterFlag=0;
                fprintf(lexDebug, "%c", character);
            }
        // <=
            else if(character=='='){
                strcpy(lexemeList[curLexemeListIndex].name, "<=");
                lexemeList[curLexemeListIndex++].tokenID = leqsym;
                nextCharacterFlag=0;
                fprintf(lexDebug, "%c", character);
            }
        // <
            else{
                strcpy(lexemeList[curLexemeListIndex].name, "<");
                lexemeList[curLexemeListIndex++].tokenID = lessym;
            }

            break;

        // >
        case 10:
            character = fgetc(ifp);
            nextCharacterFlag = 1;
            // >=
            if(character == '='){
                strcpy(lexemeList[curLexemeListIndex].name, ">=");
                lexemeList[curLexemeListIndex++].tokenID = geqsym;
                nextCharacterFlag = 0;
                fprintf(lexDebug, "%c", character);
            }

            // >
            else{
                strcpy(lexemeList[curLexemeListIndex].name, ">");
                lexemeList[curLexemeListIndex++].tokenID = gtrsym;
            }
            break;

        // ;
        case 11:
            strcpy(lexemeList[curLexemeListIndex].name, ";");
            lexemeList[curLexemeListIndex++].tokenID = semicolonsym;
            break;

        // :=
        case 12:
            character = fgetc(ifp);
            if(character == '='){
                strcpy(lexemeList[curLexemeListIndex].name, ":=");
                lexemeList[curLexemeListIndex++].tokenID = becomessym;
                fprintf(lexDebug, "%c", character);
            }
            break;
        // {
        case 13:
            strcpy(lexemeList[curLexemeListIndex].name, "{");
            lexemeList[curLexemeListIndex++].tokenID = beginsym;
            break;
        // }
        case 14:
            strcpy(lexemeList[curLexemeListIndex].name, "}");
            lexemeList[curLexemeListIndex++].tokenID = endsym;
            break;

        default:
            printf("ERROR INVALID SYMBOLS.\n");
            break;
    }

}

void print(){

    fprintf(lexDebug,"\n\nLexeme Table:\nlexeme\ttoken type\n");

    for(i = 0; i < curLexemeListIndex; i++){

    if (lexemeList[i].tokenID == 3){
        fprintf(lexDebug,"%d",lexemeList[i].numberValue);
    }else{
        fprintf(lexDebug,"%s", lexemeList[i].name);
    }

    fprintf(lexDebug,"\t\t%d\n", lexemeList[i].tokenID);


    }

    fprintf(lexDebug,"\nLexeme List:\n");
    for(i = 0; i < curLexemeListIndex; i++){

        fprintf(ofp," %d", lexemeList[i].tokenID);
        fprintf(lexDebug," %d", lexemeList[i].tokenID);

        if(lexemeList[i].tokenID == 2){
            fprintf(ofp," %s", lexemeList[i].name);
            fprintf(lexDebug," %s", lexemeList[i].name);
        }

        else if(lexemeList[i].tokenID == 3){
            fprintf(ofp," %d",lexemeList[i].numberValue);
             fprintf(lexDebug," %d",lexemeList[i].numberValue);

        }

    }

    fprintf(lexDebug,"\n\nSymbolic Representation:\n");

    for(i = 0; i < curLexemeListIndex; i++){

        fprintf(lexDebug,"%s ", symRep[lexemeList[i].tokenID]);

        if(lexemeList[i].tokenID == 2){
            fprintf(lexDebug,"%s ", lexemeList[i].name);
        }

        else if(lexemeList[i].tokenID == 3){
             fprintf(lexDebug,"%d ",lexemeList[i].numberValue);

        }

    }
}
