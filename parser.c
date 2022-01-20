//Benjamin Werner 4705456

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "info.h"

//Recommended data structure for the symbol.
typedef struct {
	int kind; 		// const = 1, var = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value)
	int level; 		// L level
	int addr; 		// M address
} symbol;

void program();
void nextToken();
void block(int lexlevel, int tableIndex);
void constdeclaration(int lexlevel, int *tableIndexPOINTER, int *dataIndexPOINTER);
void vardeclaration(int lexlevel, int *tableIndexPOINTER, int *dataIndexPOINTER);
void addSymbol(int k, int *tableIndexPOINTER, int *dataIndexPOINTER, int lexlevel);
void statement(int lexlevel, int *tableIndexPOINTER);
void condition(int lexlevel, int *tableIndexPOINTER);
void expression(int lexlevel, int *tableIndexPOINTER);
void term(int lexlevel, int *tableIndexPOINTER);
int position(char *id, int *tableIndexPOINTER, int lexlevel);
void factor(int lexlevel, int *tableIndexPOINTER);
void emit(int op, int l, int m);
void error(int errorCase);






int cx, token, num, kind;
int lexemeListIndex = 0, difference, previousDifference = 0;
char id[10];

symbol table[MAX_SYMBOL_TABLE_SIZE] = {0};
instruction code[CODE_SIZE];

void parse(void) {

    program();

		FILE* ofp = fopen("parseroutput.txt", "w");

    for (int i = 0; i < cx; i++) {
        fprintf(ofp, "%d %d %d", code[i].op, code[i].l, code[i].m);
				if (i != cx - 1)
            fprintf(ofp, "\n");
    }

    fclose(ofp);
}

void program() {

    nextToken();
    block(0, 0);

    if (token != periodsym)
        error(9);

		cx--;
		emit(11,0,3);
}

void nextToken() {

    token = lexemeList[lexemeListIndex].tokenID;

    if(token == 2)
        strcpy(id, lexemeList[lexemeListIndex].name);

    else if(token == 3)
        num = lexemeList[lexemeListIndex].numberValue;

    lexemeListIndex++;

}

void block(int lexlevel, int tableIndex) {

		int dataIndex = 4;
		int tableIndexSAVED = tableIndex;
    table[tableIndex].addr = cx;
    emit(7, 0, 0);

		if (token == constsym) {
				nextToken();

				do {

						constdeclaration(lexlevel, &tableIndex, &dataIndex);

						while(token == commasym) {
								nextToken();
								constdeclaration(lexlevel, &tableIndex, &dataIndex);
						}

						if(token != semicolonsym)
								 error(5);

						 nextToken();

				} while (token == identsym);
		}

		if (token == varsym) {
				nextToken();

				do {

						vardeclaration(lexlevel, &tableIndex, &dataIndex);

						while (token == commasym) {
								nextToken();
								vardeclaration(lexlevel, &tableIndex, &dataIndex);
						}

						if(token != semicolonsym)
								error(5);

						nextToken();

				} while(token == identsym);
		}

		while(token == procsym) {
				nextToken();


				if(token != identsym)
					error(4);

				addSymbol(3, &tableIndex, &dataIndex, lexlevel);
				nextToken();

				if(token != semicolonsym)
						error(5);

				nextToken();

				block(lexlevel + 1, tableIndex);

				if(token != semicolonsym)
							error(5);

				nextToken();
		}

    code[table[tableIndexSAVED].addr].m = cx;
    table[tableIndexSAVED].addr = cx;
    emit(6, 0, dataIndex);
    statement(lexlevel, &tableIndex);
    emit(2, 0, 0);
}

void constdeclaration(int lexlevel, int *tableIndexPOINTER, int *dataIndexPOINTER) {

    if (token == identsym) {
        nextToken();

				if (token==becomessym)
						error(1);


        if (token == eqsym)
            nextToken();

        if (token==numbersym) {
            addSymbol(1, tableIndexPOINTER,dataIndexPOINTER, lexlevel); //const
            nextToken();
        }
    }
}

void vardeclaration(int lexlevel, int *tableIndexPOINTER, int *dataIndexPOINTER) {


		if (token != identsym)
			error(4);

		addSymbol(2, tableIndexPOINTER, dataIndexPOINTER, lexlevel); //var
		nextToken();


}

void addSymbol(int k, int *tableIndexPOINTER, int *dataIndexPOINTER, int lexlevel) {

    char *id1;
    int j, len;
    (*tableIndexPOINTER)++;
    id1 = id;
    len = strlen(id);

    for (j=0;j<=len;j++) {
        table[*tableIndexPOINTER].name[j] = *id1;
        id1++;
    }

    table[*tableIndexPOINTER].kind = k;

     if (k == 1)
        table[*tableIndexPOINTER].val = num;

    else if (k == 2) {
        table[*tableIndexPOINTER].level = lexlevel;
        table[*tableIndexPOINTER].addr = *dataIndexPOINTER;
        (*dataIndexPOINTER)++;
    }

    else if (k == 3)
        table[*tableIndexPOINTER].level = lexlevel;

}

void statement(int lexlevel, int *tableIndexPOINTER) {

    int pos, cx1, cx2;

    if (token == identsym){

        pos = position(id, tableIndexPOINTER, lexlevel);

        if(pos == 0)
            error(11);

        else if (table[pos].kind != 2) {

                error(12);
                pos = 0;

        }

        nextToken();

				if (token != becomessym)
	            error(13);

	      nextToken();

        expression(lexlevel, tableIndexPOINTER);

        if (pos != 0)
            emit(4, lexlevel - table[pos].level, table[pos].addr);

    }


    else if (token == callsym) {

			nextToken();

			if (token != identsym)
				error(4);

			pos = position(id, tableIndexPOINTER, lexlevel);

			if (pos == 0)
				error(11);

			if (table[pos].kind != 3)
				error(15);

			emit(5, lexlevel - table[pos].level, table[pos].addr);

			nextToken();

    }

		//begin -> end
    else if (token == beginsym) {
        nextToken();

        statement(lexlevel, tableIndexPOINTER);

         while (token == semicolonsym) {
            nextToken();
            statement(lexlevel, tableIndexPOINTER);
         }

        if (token != endsym)
            error(17);

        nextToken();
    }

    //if -> then
    else if (token == ifsym) {
        nextToken();

        condition(lexlevel, tableIndexPOINTER);

        if(token != thensym)
					error(16);

				nextToken();

        cx1 = cx;

        emit(8, 0, 0);

        statement(lexlevel, tableIndexPOINTER);

        if(token == elsesym) {
            nextToken();

            code[cx1].m = cx + 1; //jumps past if
            cx1 = cx;
            emit(7, 0, 0);
            statement(lexlevel, tableIndexPOINTER);
        }

        code[cx1].m = cx;
    }



    //while -> do
    else if (token == whilesym) {
        cx1 = cx;
        nextToken();
        condition(lexlevel, tableIndexPOINTER);
        cx2 = cx;
        emit(8, 0, 0);
				if(token != dosym)
					error(18);

				nextToken();

        statement(lexlevel, tableIndexPOINTER);
        emit(7, 0, cx1);
        code[cx2].m = cx;
    }

		//read
		else if (token == readsym) {
				nextToken();
				emit(10, 0, 2);
				pos = position(id, tableIndexPOINTER, lexlevel);

				if(token != identsym)
					error(4);

				if (pos == 0)
					error(11);

				if (table[pos].kind != 2)
					error(12);

				if (pos != 0)
					emit (4, lexlevel - table[pos].level, table[pos].addr);

				 nextToken();
		}

    //write
    else if (token == writesym) {
        nextToken();

				if (token != identsym)
					error(4);

        expression(lexlevel, tableIndexPOINTER);
        emit(9,0,1);
    }



}

void condition(int lexlevel, int *tableIndexPOINTER) {

 if (token == oddsym) {
    nextToken();
    expression(lexlevel, tableIndexPOINTER);
    emit(2, 0, 6);

    } else {
        expression(lexlevel, tableIndexPOINTER);
        if ((token != eqsym) && (token != neqsym) && (token != lessym) && (token != leqsym) && (token != gtrsym) && (token != geqsym))
            error(20);


				int temp = token;
        nextToken();
        expression(lexlevel, tableIndexPOINTER);

        switch(temp) {
            case 9:
                emit(2, 0, 8); //EQL
                break;
            case 10:
                emit(2, 0, 9); //NEQ
                break;
            case 11:
                emit(2, 0, 10); //LSS
                break;
            case 12:
                emit(2, 0, 11); //LEQ
                break;
            case 13:
                emit(2, 0, 12); //GTR
                break;
            case 14:
                emit(2, 0, 13); //GEQ
                break;
            }
    }
}

void expression(int lexlevel, int *tableIndexPOINTER) {

    int addop;
    if (token == plussym || token == minussym) {
        addop = token;
        nextToken();

        term(lexlevel, tableIndexPOINTER);

        if(addop == minussym)
            emit(2, 0, 1);

    } else
        term (lexlevel, tableIndexPOINTER);

    while (token == plussym || token == minussym) {
        addop = token;
        nextToken();
        term(lexlevel, tableIndexPOINTER);

        if (addop == plussym)
            emit(2, 0, 2);

        else
            emit(2, 0, 3);

    }
}

void term(int lexlevel, int *tableIndexPOINTER) {

    int mulop;
    factor(lexlevel, tableIndexPOINTER);

    while(token == multsym || token == slashsym) {

        mulop = token;

        nextToken();

        factor(lexlevel, tableIndexPOINTER);

        if(mulop == multsym)
            emit(2, 0, 4);

        else
            emit(2, 0, 5);

    }
}

int position(char *id, int *tableIndexPOINTER, int lexlevel) {
	int ti = *tableIndexPOINTER;

	int curti;
	int differencenum = 0;

	while(ti != 0) {
			if (strcmp(table[ti].name, id) == 0 && table[ti].level <= lexlevel) {

					if (differencenum != 0)
							previousDifference = difference;

					difference = lexlevel - table[ti].level;

					if((differencenum == 0) || (difference < previousDifference))
							curti = ti;

					differencenum++;

			}
			ti--;
	}

	return curti;
}

void factor(int lexlevel, int *tableIndexPOINTER) {


    while ((token == identsym) || (token == numbersym) || (token == lparentsym)){

        if (token == identsym) {

            int pos = position(id, tableIndexPOINTER, lexlevel);

            if (pos == 0)
                error(11);

            if (table[pos].kind == 1)
                emit(1, 0, table[pos].val);
            else if (table[pos].kind == 2) {//var
                emit(3, lexlevel - table[pos].level, table[pos].addr); // 3 is LOD for op, lev-level is L inside LOD, adr is for M inside LOD
            }
            else
                error(21);


            nextToken();

        }

        else if(token == numbersym) {
            emit(1, 0, num);
            nextToken();
        }

        else if(token == lparentsym) {
            nextToken();
            expression(lexlevel,tableIndexPOINTER);

            if (token==rparentsym)
                nextToken();

            else
                error(22); // Right parenthesis missing.

        }
    }
}

void emit(int op, int l, int m) {

    if (cx > CODE_SIZE)
        error(28);

    else {
        code[cx].op = op; 	//opcode
        code[cx].l = l;			//lexicographical level
        code[cx].m = m;			//modifier
        cx++;
    }
}


//errors
void error(int errorCase) {

		FILE * Eofp;
  	Eofp = fopen("ERROR.txt", "w");


     printf("Error %d: ", errorCase);
     fprintf(Eofp, "Error %d: ", errorCase);

     switch (errorCase) {
         case 1:
             printf("Use = instead of :=.\n");
             fprintf(Eofp, "Use = instead of :=.\n");
             break;
         case 4:
             printf("const, var, procedure must be followed by identifier.\n");
             fprintf(Eofp, "const, var, procedure must be followed by identifier.\n");
             break;
         case 5:
             printf("Semicolon or comma missing.\n");
             fprintf(Eofp, "Semicolon or comma missing.\n");
             break;
         case 9:
             printf("Period expected.\n");
             fprintf(Eofp, "Period expected.\n");
             break;
         case 11:
             printf("Undeclared identifier.\n");
             fprintf(Eofp, "Undeclared identifier.\n");
             break;
         case 12:
             printf("Assignment to constant or procedure is not allowed.\n");
             fprintf(Eofp, "Assignment to constant or procedure is not allowed.\n");
             break;
         case 13:
             printf("Assignment operator expected.\n");
             fprintf(Eofp, "Assignment operator expected.\n");
             break;
				case 15:
		 						printf("Call of a constant or variable is meaningless.\n");
		 						fprintf(Eofp, "Call of a constant or variable is meaningless.\n");
		 						break;
         case 16:
             printf("then expected\n");
             fprintf(Eofp, "then expected\n");
             break;
				 case 17:
		 						printf("Semicolon or end expected\n");
		 						fprintf(Eofp, "Semicolon or end expected\n");
		 						break;
         case 18:
             printf("do expected\n");
             fprintf(Eofp, "do expected\n");
             break;
         case 20:
             printf("Relational operator expected.\n");
             fprintf(Eofp, "Relational operator expected.\n");
             break;
				 case 21:
		         printf("Expression must not contain a procedure identifier.\n");
		         fprintf(Eofp, "Expression must not contain a procedure identifier.\n");
		         break;
         case 22:
             printf("Right parenthesis missing.\n");
             fprintf(Eofp, "Right parenthesis missing.\n");
         default:
             printf("UNKNOWN ERROR!\n");
             fprintf(Eofp, "UNKNOWN ERROR!\n");
             break;
     }

     printf("Closing Program...");

     //leave
     exit(1);
}
