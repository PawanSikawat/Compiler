/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */




#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.c"
#include "parser.c"
#include "codegen.c"
#include "symboltable.c"
#include <time.h>

                clock_t    start_time, end_time;

                double total_CPU_time, total_CPU_time_in_seconds;
				
				bool agree = false;
				int tk;
int main(int argc, char* argv[])
{
	int choice;
	printf("\na. First and Follow set automated");
	printf("\nb. Removes comments properly");
	printf("\nc. Lexer works properly");
	printf("\nd. Parse Table populates automatically");
	printf("\ne. Parser does parsing, but doesnt writes in file");
	printf("\ne. Symbol Table implemented");
	printf("\ne. Code Generation implemented");
	printf("\n");
	do
	{
		printf("\n\nChoose one among the following:");
		printf("\n\n0. Exit");
		printf("\n1. Remove Comments");
		printf("\n2. Print Token List");
		printf("\n3. Parse Code");
		printf("\n4. Symbol Table");
		printf("\n5. Code Generation");
		printf("\n6. Compute Time Taken");
		printf("\n\nYour Choice: ");
		scanf("%d",&choice);
		done:
		printf("\n-----------------------------------------------------------------\n");
		switch(choice)
		{
			case 0 : break;
			case 1 : FILE *ipc;
					 ipc = fopen("t1-old.txt","r+");
					 FILE *opc;
					 opc = fopen("temp.txt","w");
					 if(ipc==NULL)
					 {
						 printf("Error in opening input file for comments removal!!");
						 return (-1);
					 }
					 if(opc==NULL)
					 {
						 printf("Error in opening temporary file for comments removal!!");
						 return (-1);
					 }
					 removeComments(ipc,opc);
					 fclose(ipc);
					 fclose(opc);
					 break;
			case 2 : FILE *ipl;
					 ipl = fopen("t1-old.txt","r+");
					 if(ipl==NULL)
					 {
						 printf("Error in opening input file for comments removal!!");
						 return (-1);
					 }
					 start_time = clock();
					 printf("\n\nLine_No     Lexeme                   Token_Name\n");
					while(!feof(ipl)) /*|| buffer[buff_pos]!='\0'*///)
					{
						tk = getNextToken(ipl);
						if(tk<0)
						{
							printf("\n\nLine_No     Lexeme                   Token_Name\n");
						}
						else
						{
							printTokenStream(tk+3);
						}
					}
					end_time = clock();

					total_CPU_time  =  (double) (end_time - start_time);

					total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
					
					if(agree)
					{
						printf("\nTime taken in ticks = %lf",total_CPU_time);
						printf("\nTime taken in seconds = %lf",total_CPU_time_in_seconds);
						agree = false;
					}
					fclose(ipl);
					 break;
			case 3 : FILE *ips;
					 ips = fopen(argv[1],"r+");
					 FILE *ops;
					 ops = fopen(argv[2],"w");
					 if(ips==NULL)
					 {
						 printf("Error in opening input file for comments removal!!");
						 return (-1);
					 }
					 if(ops==NULL)
					 {
						 printf("Error in opening temporary file for comments removal!!");
						 return (-1);
					 }
					 parseInputSourceCode(ips,ops);
					 fclose(ips);
					 fclose(ops);
					 break;
			case 6 : agree = true;
					 choice = 2;
					 goto done;
					 break;
			case 4 : FILE *ips;
					 ips = fopen(argv[1],"r+");
					 FILE *ops;
					 ops = fopen(argv[2],"w");
					 if(ips==NULL)
					 {
						 printf("Error in opening input file for comments removal!!");
						 return (-1);
					 }
					 if(ops==NULL)
					 {
						 printf("Error in opening temporary file for comments removal!!");
						 return (-1);
					 }
					 parseInputSourceCode(ips,ops);
					 masterSymbol(fp,head);
					 fclose(ips);
					 fclose(ops);
					 break;
			case 5 : generate_asmcode();
					 break;
			default: printf("\n\nPlease enter a valid choice!!");
					 break;
					 
		}
	}while(choice!=0);
	return 0;
}
		