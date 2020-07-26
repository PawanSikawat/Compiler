/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include <stdio.h>
#include <string.h>
#include "parser.h"


struct Stack* createStack(unsigned capacity) 
{ 
	struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); 
	stack->capacity = 1000/*capacity*/; 
	stack->top = -1; 
	//stack->array = (int*)malloc(stack->capacity * sizeof(int)); 
	return stack; 
} 

bool isFull(struct Stack* stack) 
{ 
	return stack->top == stack->capacity - 1; 
} 

bool isEmpty(struct Stack* stack) 
{ 
	return stack->top == -1; 
} 

int push(struct Stack* stack, int item) 
{ 
	if (isFull(stack)) 
		return -1; 
	stack->array[++stack->top] = item; 
	return 1;
} 
 
int pop(struct Stack* stack) 
{ 
	if (isEmpty(stack)) 
		return -1; 
	return stack->array[stack->top--]; 
} 
 
int peek(struct Stack* stack) 
{ 
	if (isEmpty(stack)) 
		return -1; 
	return stack->array[stack->top]; 
} 

bool checkPresent(int curr, int find)
{
	bool found = false;
	for(int i=1; i<MAX_FOLLOW;i++)
	{
		if(follow[curr-80][i]==0 && !found)
		{
			follow[curr-80][i] = find;
			return true;
			break;
		}
		if(follow[curr-80][i]==find)
		{
			found = true;
			break;
		}
	}
	return false;
}



						

void computeFirstAndFollowSets()
{
	int cp=GR_RULES-1;
	bool detect_eps = false;
	int index = 1;
	int pos,nt;
	bool change = true;
	bool present;
	int i,j,curr,nnt;

	///////////////////////////////////////////////  FIRST    //////////////////////////////////////////////////////////////////	
	for(i=1; i<=3; i++)
	{
		first[18][i].sym = grammar[35+i][1];
		first[18][i].rule = 35+i;
	}
	
	for(i=NT_COUNT-1; i>=0; i--)
	{
		pos=1;
		first[i][0].sym=i+80;
		if(i==18)
			continue;
		index = 1;
		detect_eps = false;
		for(j=GR_RULES-1; j>=0; j--)
		{
			index = 1;
			if((i+80)<grammar[j][0])
			{
				continue;
			}
			else if((i+80)==grammar[j][0])
			{
				do
				{					
					if(grammar[j][index]<=T_COUNT)
					{
						if(grammar[j][index]==0)
							break;
						first[i][pos].sym=grammar[j][index];
						first[i][pos].rule=j;
						pos++;
						index++;
						break;
					}
				    else 
				    {
						nt=grammar[j][index]-80;
						int k=1;
						if(first[nt][1].sym!=1)
						{
							k=1;
							while(first[nt][k].sym!=0)
							{
								first[i][pos].sym=first[nt][k].sym;
								first[i][pos].rule=j;
								pos++;
								k++;
							}
							index++;
							break;
						}
						else
						{							
							if(first[nt][1].sym==1)
							{
								k=2;
							}
							while(first[nt][k].sym!=0)
							{
								first[i][pos].sym=first[nt][k].sym;
								first[i][pos].rule=j;
								pos++;
								k++;
							}
							index++;
							if(grammar[j][index]==0 && first[nt][1].sym==1 && first[i][1].sym!=1)
							{
								while(pos>1)
								{
									first[i][pos].sym=first[i][pos-1].sym;
									first[i][pos].rule=first[i][pos-1].rule;
									pos--;
								}
								first[i][1].sym=1;
								first[i][1].rule=j;
							}
						}
						
					}
				}while(index<MAX_RHS && grammar[j][index]!=0);
			}
			else
			{
				cp=grammar[j][0];
				break;
			}
		}
	}
	/*for(int i=0; i<5; i++)
	{
		first[37][i+2].sym=0;
		first[37][i+2].rule=0;
	}*/
	

	
	////////////////////////////////////     FOLLOW    ////////////////////////////////////////////
	for(i=0;i<NT_COUNT;i++)
	{
		follow[i][0] = i+80;
	}
	follow[0][1]=2;
	//while(change)
	for(int l=0; l<30; l++)	
	{
		change = false;
		for(i=0;i<GR_RULES;i++)
		{
			for(j=1;j<MAX_RHS-1;j++)
			{
				if(grammar[i][j]==0)
				{
					break;
				}
				if(grammar[i][j]<T_COUNT)
				{
					continue;
				}
				else
				{
					detect_eps = false;
					curr = grammar[i][j];
					pos = j+1;					
					if(grammar[i][pos]==0)
					{
						end1:
						nnt = grammar[i][0]-80;
						index=1;
						while(follow[nnt][index]!=0)
						{
							change = checkPresent(curr,follow[nnt][index]);						
							index++;
						}
						continue;
					}
					if(grammar[i][pos]<=T_COUNT)
					{
						end2:
						change = checkPresent(curr,grammar[i][pos]);
						continue;
					}
					else 
					{
						int check = 4;
						do
						{
							check=0;
							nnt = grammar[i][pos];
							if(nnt==0)
								check = 1;
							else if(nnt<=T_COUNT)
								check = 2;
							else if(first[nnt-80][1].sym!=1)
								check = 3;
							else
								check = 4;
							switch(check)
							{
								case 1 : goto end1;
										 break;
								case 2 : goto end2;
										 break;
								case 3 :
								case 4 : nnt -= 80;
										 index=check-2;
										 while(first[nnt][index].sym!=0)
										 {
											change = checkPresent(curr,first[nnt][index].sym);						
											index++;
										 }
										 break;
							}
							pos++;
						}while(check==4);
					}
				}
			}
		}
	}	
}





void printFirst()
{
	
	printf("Printing First Set........!!!!");
	int k=1;
	for(int i=NT_COUNT-1;i>=0;i--)
	{
		k=1;
		printf("\n\n%s	->	",NT_SYMBOLS[i]);
		while(first[i][k].sym!=0)
		{
			printf("\t%s(%d),",T_SYMBOLS[first[i][k].sym-1], first[i][k].rule + 1);
			k++;
		}
	}
}

void printFollow()
{
	printf("Printing Follow Set........!!!!");	
	int k=1;
	for(int i=0;i<NT_COUNT;i++)
	{
		k=1;
		printf("\n\n%s	->	",NT_SYMBOLS[i]);
		while(follow[i][k]!=0)
		//for(int l=1; l<MAX_FOLLOW; l++)	
		{
			printf("\t %s,",T_SYMBOLS[follow[i][k]-1]);
			k++;
		}
	}
}
			



void printTokenStream(int tk)
{
	lexeme[++lex_len] = '\0';
	int len = 25 - strlen(lexeme);
	printf("\n%d          %s",line_count,lexeme);
	for(int i=1; i<=len; i++)
	{
		printf(" ");
	}

	printf("%s",T_SYMBOLS[tk-1]);
	token_store_node  *temp;
	temp =  (token_store_node*)malloc(sizeof(token_store_node));
	strcpy(temp->lexeme,lexeme);
	temp->line_number = line_count;
	strcpy(temp->token,T_SYMBOLS[tk-1]);
	temp->next = NULL;
	currrent ->next = temp;
}
	

						
void createParseTable()
{
	int index =1;
	bool found = false;
	for(int i=0; i<NT_COUNT; i++)
	{
		for(int j=2; j<=T_COUNT; j++)
		{
			index=1;
			found = false;
			while(first[i][index].sym!=0)
			{
				if(first[i][index].sym==j)
				{
					parseTable[i][j-2]==first[i][index].rule;
					found = true;
					break;
				}
				index++;
			}
			if(found)
				continue;
			index=1;
			if(first[i][1].sym==1)
			{
				while(follow[i][index]!=0)
				{
					if(follow[i][index]==j)
					{
						parseTable[i][j-2]==first[i][1].rule;
						found = true;
						break;
					}
					index++;
				}
			}
		}
	}
				
			
}





void reportSynError(int tk)
{
	lexeme[++lex_len] = '\0';
	printf("\n\nSyntax Error: Invalid lexeme(%s) of Token(%s) found at line no: %d",lexeme,T_SYMBOLS[tk-1],line_count);
	printf("\n\nLine_No     Lexeme                   Token_Name\n");
}


void parseInputSourceCode(FILE *fp, FILE *op)
{	head = (token_store_node*) malloc(sizeof(token_store_node));
	strcpy(head->lexeme, "-");
	head->line_number =0;
	strcpy(head->token,"T");
	head->next = NULL;
	currrent = head;
	
	computeFirstAndFollowSets();
	createParseTable();
	//struct Stack* stack = createStack(1000);     ///////////-----------CHOOSE SIZE APPROPRIATELY
	struct Stack* stack = createStack(1000); 
	//int tk_stream[1000];
	int top = 0;
	int stop,tk;
	int popped;
	int rule_no=0;
	int rule_len=0;
	bool rf = false;
	int pop_temp;
	bool err_rec = false;

	//push(stack, 10);  
	push(stack, 80);
	stop=80;
	
	while(!feof(fp) /*|| buffer[buff_pos]!='\0'*/)
	{
		rf = false;
		tk = getNextToken(fp)+3;
		if(tk<2)
		{
			if(tk==1)
				printf("\n\nLine_No     Lexeme                   Token_Name\n");
			

		}
		else
		{
			printTokenStream(tk);
			//if(!err_rec)
			//{
				empty_rule:
				stop = peek(stack);
				if(stop==-1)
				{
					if(feof(fp))
					{
						printf("Syntactically correct code....!!!");
						break;
					}
					else
					{
						printf("Syntax Error: Code contains extra symbols/segments!!");
						break;
					}
				}
				//empty_rule:
				if(stop<T_COUNT)
				{
					if(stop==tk)
					{
						popped = pop(stack);
					}
					else
						reportSynError(tk);
				}
				else
				{
					rule_no = parseTable[stop][tk];
					if(rule_no==0)
					{
						err_rec = true;
						reportSynError(tk);
					}
					else
					{
						if(grammar[rule_no][1]==1)
						{
							pop_temp = pop(stack);
							//goto empty_rule;
						}
						else
						{
							rule_len = 0;
							int index = 1;
							while(grammar[rule_no][index]!=0)
							{
								rule_len++;
								index++;
							}
							pop_temp = pop(stack);
							index = rule_len;
							while(index!=0)
							{
								push(stack,grammar[rule_no][index]);
								index--;
							}
						}
					/*int k=1;	
					while(first[stop-80][k].sym!=0);
					{
						if(first[stop-80][k].sym==tk)
						{
							rule_no=first[stop-80][k].rule;
							rf = true;
							break;
						}
						k++;
					}
					if(rf)
					{
						rule_len = 0;
						int index = 1;
						while(grammar[rule_no][index]!=0)
						{
							rule_len++;
							index++;
						}
						pop_temp = pop(stack);
						index = rule_len;
						while(index!=0)
						{
							push(stack,grammar[rule_no][index]);
							index--;
						}
					}
					else
					{
						if(first[stop][1].sym==1)
						{
							pop_temp = pop(stack);
						}
						else
						{
							err_rec = true;
							reportSynError(tk);
						}
					}*/
						
				    }
			/*}
			else
			{
				//if(condition satisfy) -> err_rec = false;
					
			}*/
			
			}
		}		
					
					
			
	}
	
}				
							