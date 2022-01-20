    //Benjamin Werner UCFID 4705456

    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<strings.h>
    #include "info.h"


    //Initial values for PM/0 CPU registers:
    int sp = MAX_STACK_HEIGHT;
    int bp = MAX_STACK_HEIGHT - 1;
    int pc = 0;
    int flag = 0;

    //initialize print flag taken from main, used to determine if we should print to screen.
    int printflag;

    //Initialize stack
    int stack[MAX_STACK_HEIGHT] = {0};

    //Create a array which keeps track of the positions of each activation record.
    int activationRecords[CODE_SIZE];
    int recordCounter = 0;

    void interpretVM();
    void performVM();
    void executeStep();
    void printVM();
    void read();
    int base(int l, int base);

    //Initialize array of our code as well as a register.
    instruction code[CODE_SIZE];
    instruction ir;

    //Declare file io variables.
    FILE *ifp;
    FILE *ofp;


    //Main function
    void vm(int vmflag){
        printflag = vmflag;


        //Try to the file directed in the Readme
        ifp = fopen("parseroutput.txt", "r");

        if(ifp == NULL){
            printf("\nERROR: PARSEROUTPUT DOESN'T EXIST!!!\n");
            exit(1);
        }

        ofp = fopen("vmoutput.txt", "w");

        //interpret and print code from input file
        interpretVM();

        //perform the fetch and execution steps
        performVM();

        fclose(ifp);
        fclose(ofp);
    }


    //sets ifp info to the code array and prints the interpreted assembly code
    void interpretVM(){

        int i;

        fprintf(ofp, "\nInterpreted Assembly Language\n");
        if(printflag)printf("Interpreted Assembly Language\n");
        fprintf(ofp, "\nLine\tOP\tL\tM");
        if(printflag)printf("\nLine\tOP\tL\tM");
        //Read in the lines from the input file and print to the output file the interpreted assembly language results.
        for(i = 0; !feof(ifp); i++){
            //First read in the data into the code array
            fscanf(ifp, "%d", &code[i].op);
            fscanf(ifp, "%d", &code[i].l);
            fscanf(ifp, "%d", &code[i].m);

            //Given the op code display appropriate operation
            int op = code[i].op;
            switch(op){
                case 1: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "lit\t");
                        if(printflag){
                                printf("\n%d\t", i);
                                printf("lit\t");
                        }
                        break;

                case 2: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "opr\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("opr\t");
                        }
                        break;

                case 3: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "lod\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("lod\t");
                        }
                        break;

                case 4: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "sto\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("sto\t");
                        }
                        break;

                case 5: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "cal\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("cal\t");
                        }
                        break;

                case 6: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "inc\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("inc\t");
                        }
                        break;

                case 7: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "jmp\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("jmp\t");
                        }
                        break;

                case 8: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "jpc\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("jpc\t");
                        }
                        break;

                case 9: fprintf(ofp, "\n%d\t", i); fprintf(ofp, "sio\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("sio\t");
                        }
                        break;

                case 10:fprintf(ofp, "\n%d\t", i); fprintf(ofp, "sio\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("sio\t");
                        }
                        break;

                case 11:fprintf(ofp, "\n%d\t", i); fprintf(ofp, "sio\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("sio\t");
                        }
                        break;

                default:fprintf(ofp, "\n%d\t", i); fprintf(ofp, "ERROR INVALID OP CODE\t");
                        if(printflag){
                            printf("\n%d\t", i);
                            printf("ERROR INVALID OP CODE\t");
                        }
                        break;
            }

            //Display L and M
            fprintf(ofp, "%d\t", code[i].l);
            if(printflag)printf("%d\t", code[i].l);
            fprintf(ofp, "%d\t", code[i].m);
            if(printflag)printf("%d\t", code[i].m);
        }
    }

    //Performs fetch and execute cycle
    void performVM(){
        int i, j;

        fprintf(ofp, "\n\nExecution of the Program: \n");
        fprintf(ofp, "\t\t\tpc\tbp\tsp\tstack\n");
        fprintf(ofp, "Initial Values \t\t%d\t%d\t%d\n", pc, bp, sp);
        if(printflag)printf("\n\nExecution of the Program: \n");
        if(printflag)printf("\t\t\tpc\tbp\tsp\tstack\n");
        if(printflag)printf("Initial Values \t\t%d\t%d\t%d\n", pc, bp, sp);

        //Fetch and Execute step, runs until we hit a flag
        while(flag == 0){

            //fetch step, set the register and increment
            ir = code[pc];
            fprintf(ofp, "%d ", pc);
            if(printflag)printf("%d ", pc);
            pc++;

            //execute
            executeStep();

            //print out data
            fprintf(ofp, "%d %d\t\t", ir.l, ir.m);
            fprintf(ofp, "%d\t%d\t%d\t", pc, bp, sp);
            if(printflag)printf("%d %d\t\t", ir.l, ir.m);
            if(printflag)printf("%d\t%d\t%d\t", pc, bp, sp);

            if(flag)
                return;
            //print out stack
            for(i = MAX_STACK_HEIGHT-1; i >=  sp; i--){
                fprintf(ofp, "%d ", stack[i]);
                if(printflag)printf("%d ", stack[i]);
                for (j = 0; j < recordCounter; j++){
                    if (i == activationRecords[j] && i != sp){
                        fprintf(ofp, "| ");
                        if(printflag)printf("| ");
                    }
                }
            }
            //Prepares for a new Fetch/Execute step
            fprintf(ofp, "\n");
            if(printflag)printf("\n");
        }
    }

    //Uses the current register to execute its instruction via its op code.
    void executeStep(){

        switch(ir.op){
        //01   –   LIT      0, M    Push constant value (literal) M onto the stack
        case 1:
            fprintf(ofp, "lit ");
            if(printflag)printf("lit ");
            sp = sp - 1;
            stack[sp] = ir.m;
            break;

        //02   –   OPR    0, M  Operation to be performed on the data at the top of the stack
        case 2:
                switch(ir.m){
                case 0:
                    fprintf(ofp, "ret ");
                    if(printflag)printf("ret ");
                    sp = bp + 1;
                    pc = stack[sp - 4];
                    bp = stack[sp - 3];
                    break;

                case 1:
                    fprintf(ofp, "neg ");
                    if(printflag)printf("neg ");
                    stack[sp]--;
                    break;

                case 2:
                    fprintf(ofp, "add ");
                    if(printflag)printf("add ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] + stack[sp - 1];
                    break;

                case 3:
                    fprintf(ofp, "sub ");
                    if(printflag)printf("sub ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] - stack[sp - 1];
                    break;

                case 4:
                    fprintf(ofp, "mul ");
                    if(printflag)printf("mul ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] * stack[sp - 1];
                    break;

                case 5:
                    fprintf(ofp, "div ");
                    if(printflag)printf("div ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] / stack[sp - 1];
                    break;

                case 6:
                    fprintf(ofp, "odd ");
                    if(printflag)printf("odd ");
                    stack[sp] = stack[sp] % 2;
                    break;

                case 7:
                    fprintf(ofp, "mod ");
                    if(printflag)printf("mod ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] % stack[sp - 1];
                    break;

                case 8:
                    fprintf(ofp, "eql ");
                    if(printflag)printf("eql ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] == stack[sp - 1];
                    break;

                case 9:
                    fprintf(ofp, "neq ");
                    if(printflag)printf("neq ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] != stack[sp - 1];
                    break;

                case 10:
                    fprintf(ofp, "lss ");
                    if(printflag)printf("lss ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] < stack[sp - 1];
                    break;

                case 11:
                    fprintf(ofp, "leq ");
                    if(printflag)printf("leq ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] <= stack[sp - 1];
                    break;

                case 12:
                    fprintf(ofp, "gtr ");
                    if(printflag)printf("gtr ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] > stack[sp - 1];
                    break;

                case 13:
                    fprintf(ofp, "geq ");
                    if(printflag)printf("geq ");
                    sp = sp + 1;
                    stack[sp] = stack[sp] >= stack[sp - 1];
                    break;

                }
            break;

        //03   –   LOD   L, M    Load value to top of stack from the stack location at offset M fromL lexicographical levels down
        case 3:
            fprintf(ofp, "lod ");
            if(printflag)printf("lod ");
            sp = sp - 1;
            stack[sp] = stack[base(ir.l, bp) - ir.m];
            break;

        //04   –   STO    L, M    Store value at top of stack in the stack location at offset M fromL lexicographical levels down
        case 4:
            fprintf(ofp, "sto ");
            if(printflag)printf("sto ");
            stack[base(ir.l, bp) - ir.m] = stack[sp];
            sp = sp + 1;
            break;

        //05   –   CAL    L, M    Call procedure at code index M (generates new Activation Record
        case 5:
            fprintf(ofp, "cal ");
            if(printflag)printf("cal ");
            activationRecords[recordCounter++] = sp;
            stack[sp - 1] = 0;
            stack[sp - 2] = base(ir.l, bp);
            stack[sp - 3] = bp;
            stack[sp - 4] = pc;
            bp = sp - 1;
            pc = ir.m;
            break;

        //06   –   INC0, M  Allocate M locals (increment sp by M).
        case 6:
            fprintf(ofp, "inc ");
            if(printflag)printf("inc ");
            sp = sp - ir.m;
            break;

        //07   –   JMP    0, M  Jump to instruction M
        case 7:
            fprintf(ofp, "jmp ");
            if(printflag)printf("jmp ");
            pc = ir.m;
            break;

        //08   –   JPC0, M  Jump to instruction M if top stack element is 0
        case 8:
            fprintf(ofp, "jpc ");
            if(printflag)printf("jpc ");
            if(stack[sp]==0)
                pc = ir.m;

            sp = sp + 1;
            break;
        //09   –   SIO0, 1  Write the top stack element to the screen
        case 9:
            printVM();
            sp = sp + 1;
            break;
        //10   –   SIO0, 2  Read in input from the user and store it on top of the stack.
        case 10:

            sp = sp - 1;

            read();

            break;

        //SIO - End of Program
        case 11:
            fprintf(ofp, "sio ");
            if(printflag)printf("sio ");
            flag = 1;
            //break;
            return;


        default:
            fprintf(ofp, "ERROR INVALID OP CODE ");
            if(printflag)printf("ERROR INVALID OP CODE ");
        return;

        }

    return;

    }

    //Prints the top of the stack
    void printVM(){
        fprintf(ofp, "\tprint stack called: stack[%d] = %d\n", sp, stack[sp]);
        fprintf(ofp, "%d sio ", (pc-1));
        printf("\n\n\tprint stack called: stack[%d] = %d\n\n", sp, stack[sp]);
        printf("%d sio ", (pc-1));
    }

    //Prompts the user to set the top of the stack
    void read(){

        printf("\n\n\tPlease input a integer you want on the top of the stack: ");
        int temp;
        scanf("%d", &temp);
        stack[sp] = temp;
        fprintf(ofp, "sio ");
        printf("\n%d sio ", (pc-1));


    }

    //Find base L level down
    int base(int l, int base){

        int b1; //Find base L levels down

        b1 = base;

        while(l > 0){
            b1 = stack[b1 - 1];
            l--;
        }
        return b1;
    }
