/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

int hashvaluer(char *word)
{
    int bottom= 0;
    int mid;
    int top = TOKENCOUNT-1;

    while(bottom <= top){
        mid = (bottom + top)/2;
        if (strcmp(hashtable[mid].name, word) == 0){
           return hashtable[mid].value;
        }
        if (strcmp(hashtable[mid].name, word) >0){
            top= mid - 1;
        }
        if (strcmp(hashtable[mid].name, word) <0){
            bottom= mid + 1;
        }
    }
    return 54;
}

void getStream(FILE *fp)
{
	if(end_file)
	{
		printf("\n Compilation Complete!! \n");
		return;
	}
	
	//Setting file pointer to the last read location
    fseek(fp,fileRead_count * buff_size,SEEK_SET);
	
	int count = 0;
	while(!feof(fp) && count < buff_size)
	{
		buffer[count] = fgetc(fp);
		count++;
	}
	
	if(feof(fp))
		end_file = true;
	
	fileRead_count++;
	
	//random
	printf("\n Perfectly Executed!!\n");
}



void removeComments(FILE *ip,FILE *op)
{
	char ch,ext;
	while(!feof(ip))
	{
		ch=fgetc(ip);
		if(feof(ip))
		{
			fputc('$',op);
			break;
		}
		if(ch=='*')
		{
			ext=fgetc(ip);
			if(ext=='*')
			{
				in_comment:
				ch=fgetc(ip);
				while(ch!='*' && !feof(ip))
				{
					if(ch=='\n')
						fputc(ch,op);
					ch=fgetc(ip);
				}
				if(feof(ip))
				{
					fputc('$',op);
					break;
				}
				if(ch=='*')
				{
					ext=fgetc(ip);
					if(ext=='*')
						goto again;
					if(ext=='\n')
						fputc(ext,op);	
					if(feof(ip))
					{
						fputc('$',op);
						break;
					}
					goto in_comment;
				}								
			}
			else
			{
				fputc(ch,op);
				fputc(ext,op);
				goto again;
			}
		}
		fputc(ch,op);
		again:
		if(feof(ip))
		{
			fputc('$',op);
			break;
		}
	}
	printf("\n Comments removed successfully!!\n");
}
		
		
		
/*void inc_buff_pos(FILE *fp)
{
	buff_pos++;
	if(buff_pos == buff_size)
	{
		buff_pos = 0;
		getStream(fp);
	}
}*/



void reportLexError(int count, char *err)
{
	char ch[2];
	err[count] = '\0';
	printf("\n\nLexical Error: inappropriate input '%s' found at line no: %d\n",err,line_count);
	printf("\n\nLine_No     Lexeme                   Token_Name\n");
	/*if(count==1)
	{
		printf("\n\nLexical Error: inappropriate input '%c' found at line no: %d\n",buffer[buff_pos],line_count);
	}
	else 
	{
		printf("\n\nLexical Error: inappropriate input '%c%c' found at line no: %d\n",buffer[buff_pos],buffer[buff_pos++],line_count);
	}
	buff_pos++;*/
}



/*int idORkey(char* arr)
{
	//Write code here
}
*/



int getNextToken(FILE *fp)  //return(-4): white space   //return(-2): lexical error   //return(-3): default condition
{	
	//char ch = buffer[buff_pos];
	if(dd)
	{
		lex_len=2;
		lexeme[0] = '.';
		lexeme[1] = '.';
		lexeme[2] = '\0';
		dd = false;
		return 43;
	}
		
	if(dont)
	{
		ch = fgetc(fp);
	}
	dont = false;
	char ext;
	lex_len = 0;
	int i;
	char err[3];
	switch(ch)
	{
		case 'a' ... 'z':
		case 'A' ... 'Z':	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							while(ch>='a' && ch<='z' || ch>='A' && ch<='Z' || ch>='0' && ch<='9' || ch=='_')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								//ch = buffer[buff_pos];
								ch = fgetc(fp);
							}
							lexeme[lex_len+1] = '\0';
							i = hashvaluer(lexeme)-3;
							return i;
							break;
		case '['		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 47;
							break;
		case ']'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 48;
							break;					
		case '('		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 49;
							break;				
		case ')'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 50;
							break;				
		case '+'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 30;
							break;
		case '-'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 31;
							break;
		case '/'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 33;
							break;
		case 32			:			
		case '\t'		:	//inc_buff_pos(fp);
							dont = true;
							return(-4);
							break;					
		case '\n'		:	//inc_buff_pos(fp);
							line_count++;
							dont = true;
							return(-4);
							break;					
		case ','		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 45;
							break;					
		case ';'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							dont = true;
							return 44;
							break;	
		case '='		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							if(ch == '=')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								dont = true;
								return 38;
							}
							else
							{
								//buff_pos--;
								err[0] = lexeme[lex_len];
								reportLexError(1,err);
								return(-2);
							}
							break;	
		case '!'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							if(ch == '=')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								dont = true;
								return 39;
							}
							else
							{
								//buff_pos--;
								err[0] = lexeme[lex_len];
								reportLexError(1,err);
								return(-2);
							}
							break;
		case '.'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							if(ch == '.')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								dont = true;
								return 43;
							}
							else
							{
								//buff_pos--;
								err[0] = lexeme[lex_len];
								reportLexError(1,err);
								return(-2);
							}
							break;
		case '<'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							if(ch == '=')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								dont = true;
								return 35;
							}
							else if(ch == '<')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								//ch = buffer[buff_pos];
								ch = fgetc(fp);
								if(ch == '<')
								{
									lex_len++;
									lexeme[lex_len] = ch;
									//inc_buff_pos(fp);
									dont = true;
									return 52;
								}
								else
									return 40;
							}
							else
							{
								return 34;
							}
							break;						
		case '>'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							if(ch == '=')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								dont = true;
								return 36;
							}
							else if(ch == '>')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								//ch = buffer[buff_pos];
								ch = fgetc(fp);
								if(ch == '>')
								{
									lex_len++;
									lexeme[lex_len] = ch;
									//inc_buff_pos(fp);
									dont = true;
									return 53;
								}
								else
									return 41;
							}
							else
							{
								return 37;
							}
							break;					
		case ':'		:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							if(ch == '=')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								dont = true;
								return 46;
							}
							else
							{
								return 42;
							}
							break;	
		case '*'        :   lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							if(ch=='*')
							{
								comm:
								//inc_buff_pos(fp);
								//ch = buffer[buff_pos];
								ch = fgetc(fp);
								while(ch!='*')
								{
									if(ch=='\n')
										line_count++;
									//inc_buff_pos(fp);
									//ch = buffer[buff_pos];
									ch = fgetc(fp);
								}
								if(ch=='*')
								{
									//inc_buff_pos(fp);
									//ch = buffer[buff_pos];
									ch = fgetc(fp);
									if(ch=='*')
									{
										dont = true;
										return(-1);
										break;
									}
									else
										goto comm;
								}
							}
							else
							{
								return 32;
							}
							break;
		case '0' ... '9':	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//ch = buffer[buff_pos];
							ch = fgetc(fp);
							while(ch>='0' && ch<='9')
							{
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								//ch = buffer[buff_pos];
								ch = fgetc(fp);
							}
							if(ch=='.')
							{
								//int p = ftell(fp);
								lex_len++;
								lexeme[lex_len] = ch;
								//inc_buff_pos(fp);
								//ch = buffer[buff_pos];
								ch = fgetc(fp);
								if(ch=='.')
								{
									lex_len--;
									dd = true;
									dont = true;
									//fseek(fp,p,SEEK_CUR);
									//buff_pos--;
									return 54;
								}
								else if(ch>='0' && ch<='9')
								{
									lex_len++;
									lexeme[lex_len] = ch;
									//inc_buff_pos(fp);
									//ch = buffer[buff_pos];
									ch = fgetc(fp);
									while(ch>='0' && ch<='9')
									{
										lex_len++;
										lexeme[lex_len] = ch;
										//inc_buff_pos(fp);
										//ch = buffer[buff_pos];
										ch = fgetc(fp);
									}
									if(ch=='E' || ch=='e')
									{
										int sign = 0;
										lex_len++;
										lexeme[lex_len] = ch;
										//inc_buff_pos(fp);
										//ch = buffer[buff_pos];
										ch = fgetc(fp);
										if(ch=='+' || ch=='-')
										{
											lex_len++;
											lexeme[lex_len] = ch;
											//inc_buff_pos(fp);
											//ch = buffer[buff_pos];
											ch = fgetc(fp);
											sign = 1;
										}
										if(ch>='0' && ch<='9')
										{
											while(ch>='0' && ch<='9')
											{
												lex_len++;
												lexeme[lex_len] = ch;
												//inc_buff_pos(fp);
												//ch = buffer[buff_pos];
												ch = fgetc(fp);
											}
											return 55;
										}
										else
										{
											//lex_len = lex_len - (sign+1);
											//buff_pos = buff_pos - (sign+1);
											if(sign==0)
											{
												err[0] = lexeme[lex_len];
												reportLexError(1,err);
											}
											else
											{
												err[0] = lexeme[lex_len-1];
												err[1] = lexeme[lex_len];
												reportLexError(2,err);
											}
											lex_len = lex_len - (sign+1);
											//reportLexError(sign+1);
											return 55;
										}
											
									}
									else 
									{
										return 55;
									}
								}
								else
								{									
									//buff_pos--;
									err[0] = lexeme[lex_len];
									reportLexError(1,err);
									//reportLexError(1);
									lex_len--;
									return 54;
								}
							}
							else
							{
								return 54;
							}
							break;
		default			:	lexeme[lex_len] = ch;
							//inc_buff_pos(fp);
							//reportLexError(1);
							return(-2);
							break;
	}
}
