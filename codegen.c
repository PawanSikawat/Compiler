/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include "codegen.h"
#include "semantic.h"
#include "symboltable.h"
#include "ast.h"
#include "parser.h"

int data_bytes[] = {2, 4, 1};

void generate_asmcode_util(char* str)
{
	asm_file = fopen(str,"w");
	if(asm_file == NULL)
		printf("Cannot open file called:  %s\n Please check again!!",str);
	else
		generate_asmcode();
}

void generate_asmcode()
{
	//start creation of assembly code here...
	//header of the asm code ->

	/* ;Start of assembly code
	section .data
	formatin : db "%d", 0
	formatout : db "%d", 10, 0
	section .bss
	section .text
	 extern printf
	 extern scanf
	 GLOBAL main
	main:
	*/

	fprintf(asm_file,";Start of assembly code");
	fprintf(asm_file,"section .data\n");
	fprintf(asm_file,"formatin : db \"%%d\", 0\n"); // required for usng printf and scanf
	fprintf(asm_file,"formatout : db \"%%d\", 10, 0\n");
	fprintf(asm_file,"section .bss\n");
	//set marker label to 1 -> This will used to create jump statement lables in the asm code.
	marker = 1;
	generate_data(broot);
	fprintf(asm_file,"section .text\n extern printf\n extern scanf\n GLOBAL main\nmain:\n"); //external c libraries to use printf and scanf in the code
	generate_asm(astroot,broot);
	fclose(asm_file);
}

void variable_get(char* name, astptr aptr) //Get name for the variable
{
	strcpy(name , "l");
	varptr vptr = get_id(aptr->name.tok.lexeme,aptr->scope);
	strcat(name,aptr->name.tok.lexeme);
	char line[50];
	sprintf(line , "%d" , vptr->var.line);
	strcat(name, line);
}

void l_traverse(astptr aptr) //Traverse left
{
	if(aptr->type == INT){
		fprintf(asm_file,"mov ax,dx\n");
		fprintf(asm_file,"movsx rax,ax\n");
		fprintf(asm_file,"push rax\n");
	}
		
	
	if(aptr->type == BOOL){
		fprintf(asm_file,"mov al,dl\n");
		fprintf(asm_file,"movsx rax,ax\n");
		fprintf(asm_file,"push rax\n");
	}
}

void r_traverse(astptr aptr) //Traverse right
{
	if(aptr->type == INT)
		fprintf(asm_file,"mov bx,dx\n");

	if(aptr->type == BOOL)
		fprintf(asm_file,"mov bl,dl\n");
}

void get_switch_id(char* name,astptr a)
{
	while(a->name.c != conditionalStmt)
		a = a->parent;
	variable_get(name,a->children[0]);
}

int get_switch_end(astptr a)
{
	while(a->name.c != conditionalStmt)
		a = a->parent;
	return a->end;
}

void operations(astptr a_tok,typ var_type)
{
	if(var_type == BOOL)
	{
		if(a_tok->name.tok.type == AND)
		{
			fprintf(asm_file,"pop rax\n");
			fprintf(asm_file,"and dl,al\n");
		}
		else if(a_tok->name.tok.type == OR)
		{
			fprintf(asm_file,"pop rax\n");
			fprintf(asm_file,"or dl,al\n");
		}
	}

	else if(var_type == INT)
	{

		switch(a_tok->name.tok.type){
			case PLUS:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"add dx,ax\n");
				break;
			case MINUS:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"neg dx\n");
				fprintf(asm_file,"add dx,ax\n");
				break;
			case MUL:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"imul dx\n");
				fprintf(asm_file,"mov dx,ax\n");
				break;
			case DIV:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file, "mov si,dx\n");
				fprintf(asm_file,"mov dx,0\n");
				fprintf(asm_file,"idiv si\n");
				fprintf(asm_file,"mov dx,ax\n");
				break;
			case LT:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"cmp ax,dx\n");
				fprintf(asm_file,"jl L%d\n",marker);
				fprintf(asm_file,"mov dl,0\n");
				fprintf(asm_file,"jmp L%d\n",marker+1);
				fprintf(asm_file,"L%d: mov dl,1\n", marker);
				fprintf(asm_file,"L%d:\n",marker+1);
				//update marker
				marker+=2;
				break;
			case LE:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"cmp ax,dx\n");
				fprintf(asm_file,"jle L%d\n", marker);
				fprintf(asm_file,"mov dl,0\n");
				fprintf(asm_file,"jmp L%d\n",marker+1);
				fprintf(asm_file,"L%d: mov dl,1\n",marker);
				fprintf(asm_file,"L%d: \n",marker+1);
				//update marker
				marker += 2;
				break;
			case GT:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"cmp ax,dx\n");
				fprintf(asm_file,"jg L%d\n",marker);
				fprintf(asm_file,"mov dl,0\n");
				fprintf(asm_file,"jmp L%d\n",marker+1);
				fprintf(asm_file,"L%d: mov dl,1\n", marker);
				fprintf(asm_file,"L%d:\n",marker+1);
				//update marker
				marker+=2;
				break;
			case GE:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"cmp ax,dx\n");
				fprintf(asm_file,"jge L%d\n", marker);
				fprintf(asm_file,"mov dl,0\n");
				fprintf(asm_file,"jmp L%d\n",marker+1);
				fprintf(asm_file,"L%d: mov dl,1\n",marker);
				fprintf(asm_file,"L%d: \n",marker+1);
				//update marker
				marker += 2;
				break;
			case EQ:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"cmp ax,dx\n");
				fprintf(asm_file,"je L%d\n", marker);
				fprintf(asm_file,"mov dl,0\n");
				fprintf(asm_file,"jmp L%d\n",marker+1);
				fprintf(asm_file,"L%d: mov dl,1\n",marker);
				fprintf(asm_file,"L%d: \n",marker+1);
				//update marker
				marker += 2;
				break;
			case NE:
				fprintf(asm_file,"pop rax\n");
				fprintf(asm_file,"cmp ax,dx\n");
				fprintf(asm_file,"jne L%d\n", marker);
				fprintf(asm_file,"mov dl,0\n");
				fprintf(asm_file,"jmp L%d\n",marker+1);
				fprintf(asm_file,"L%d: mov dl,1\n",marker);
				fprintf(asm_file,"L%d: \n",marker+1);
				//update marker
				marker += 2;
				break;
		}
	}	
}

void generate_data(bptr broot)
{
	int i =0;
	while(i<100)
	{
		varptr place = broot->var_table[i]; 
		while(place != NULL)
		{
			variable v = place->var;

			if(v.type.isarr==TRUE) // If its an array
			{
				int num_elements = v.type.high - v.type.low + 1;
				char varname[50];
				strcpy(varname , v.name);
				sprintf(varname , "%s%d" , varname, v.line);

				int type_var = v.type.type;

				switch(type_var){
					case INT: fprintf(asm_file, "1%s: resw %d\n",varname, num_elements);
						break;
					case RE: fprintf(asm_file,"1%s: resd %d\n",varname,num_elements);
						break;
					case BOOL: fprintf(asm_file , "l%s: resb %d\n" , varname , num_elements);
						break;
				}

				for(int j = 0 ; j < num_elements - 1 ; j++) //Store all the elements
					place = place->next;

			}
			else if(v.type.isarr==FALSE) //Not an array
			{
				char varname[50];
				strcpy(varname , v.name);
				sprintf(varname , "%s%d" , varname, v.line);

				int var_type = v.type.type;

				switch (var_type)
				{
				case INT:
					fprintf(asm_file , "l%s: resw 1\n" , varname);
					break;
				case RE:
					fprintf(asm_file , "l%s: resd 1\n" , varname);
					break;
				case BOOL:
					fprintf(asm_file , "l%s: resb 1\n" , varname);
					break;
				}
					
			}
			place = place->next;
		}
	}

	for(i = 0 ; i < broot->no_of_children ; i++) //Recursive call for all the data
		generate_data(broot->children[i]);
}

void check_pro(astptr aroot, bptr broot){
	if(!aroot->children[0]->children[0]->is_terminal)
			{
				printf("ERROR : Declerations is done before the definition of the modules.. Please check your code!!\n");
				return;
			}
			else if(!aroot->children[1]->children[0]->is_terminal)
			{
				printf("ERROR : Definition is done before the decelration of the modules.. Please check your code!!\n");
				return;
			}
			else if(!aroot->children[3]->children[0]->is_terminal)
			{
				printf("ERROR : Definition can not be done after decelration of the modeules.. Please check your code!!\n");
				return;
			}

			generate_asm(aroot->children[2],broot);

			fprintf(asm_file , "mov ebx,0\n");
			fprintf(asm_file,"mov eax,1\n");
			fprintf(asm_file,"int 0x80\n");
}

void check_iostmt(astptr aroot, bptr broot){
	if(aroot->no_of_children == 2)
			{
				typ t = aroot->children[0]->type;
				char name[50];
				variable_get(name,aroot->children[0]);
				if(aroot->children[1]->children[0]->is_terminal)
				{
					varptr vt = get_id(aroot->children[0]->name.tok.lexeme,aroot->children[0]->scope);
					if(vt->var.type.isarr)
			    	{
			    		int curr_marker = marker;
			    		fprintf(asm_file,"mov bp,%d\nL%d:\n",vt->var.type.low,marker);
			    		marker++;
			    		fprintf(asm_file,"cmp bp,%d\njg L%d\n",vt->var.type.high,marker);
			    		marker++;
			    		int index_low = vt->var.type.low;
			    		
			    		fprintf(asm_file,"movzx rsi,bp\n");
			    		fprintf(asm_file,"sub rsi,%d\n",index_low);
			    		fprintf(asm_file,"mov rdi,formatin\n");
						fprintf(asm_file,"lea rsi,[%s+rsi*%d]\n",name,data_bytes[(int)t]);
						fprintf(asm_file,"mov al,0\ncall scanf\n");
						fprintf(asm_file,"inc bp\njmp L%d\nL%d:\n",curr_marker,curr_marker+1);
			    	}
			    	else
			    	{
			    		fprintf(asm_file,"mov rdi,formatin\n");
						fprintf(asm_file,"lea rsi,[%s]\n",name);
						fprintf(asm_file,"mov al,0\ncall scanf\n");
					}
				}
				else
				{
					astptr index_node = aroot->children[1]->children[0];
					int index_val;
					varptr vt = get_id(aroot->children[0]->name.tok.lexeme,broot);
					int index_low = vt->var.type.low;

					if(index_node->children[0]->name.tok.type == NUM)
					{
						index_val = get_int(index_node->children[0]->name.tok.lexeme);
						int offset = (index_val - index_low)*data_bytes[(int)t];
						fprintf(asm_file,"lea rsi,[%s+%d]\n",name,offset);
					}
					else
					{
						char ind_name[50];
						variable_get(ind_name,index_node->children[0]);
						if(t == INT)
							fprintf(asm_file,"movzx rsi,word[%s]\n",ind_name);
						else if(t == BOOL)
							fprintf(asm_file,"movzx rsi,byte[%s]\n",ind_name);
						fprintf(asm_file,"sub rsi,%d\n",index_low);
						fprintf(asm_file,"mov rdi,formatin\n");
						fprintf(asm_file,"lea rsi,[%s+rsi*%d]\n",name,data_bytes[(int)t]);
						fprintf(asm_file,"mov al,0\ncall scanf\n");
					}
				}
				
			}
			else
			{
				if(aroot->children[0]->children[0]->name.tok.type == ID)
				{
					astptr idnode = aroot->children[0]->children[0]; 
					typ t = idnode->type;
				    char name[50];
				    variable_get(name,idnode);
				    if(aroot->children[0]->children[1]->children[0]->is_terminal)
				    {
				    	varptr vt = get_id(idnode->name.tok.lexeme,idnode->scope);
				    	if(vt->var.type.isarr)
				    	{
				    		int curr_marker = marker;
				    		fprintf(asm_file,"mov bp,%d\nL%d:\n",vt->var.type.low,marker);
				    		marker++;
				    		fprintf(asm_file,"cmp bp,%d\njg L%d\n",vt->var.type.high,marker);
				    		marker++;
				    		int index_low = vt->var.type.low;
				    		
				    		fprintf(asm_file,"movzx rsi,bp\n");
				    		fprintf(asm_file,"sub rsi,%d\n",index_low);
				    		fprintf(asm_file,"mov rdi,formatout\n");
				    		if(t == INT)
								fprintf(asm_file,"movsx rsi,word[%s+rsi*%d]\n",name,data_bytes[(int)t]);
							else if(t == BOOL)
								fprintf(asm_file,"movsx rsi,byte[%s+rsi*%d]\n",name,data_bytes[(int)t]);
							fprintf(asm_file,"mov al,0\ncall printf\n");
							fprintf(asm_file,"inc bp\njmp L%d\nL%d:\n",curr_marker,curr_marker+1);
				    	}
				    	else
				    	{
				    		fprintf(asm_file,"mov rdi,formatout\n");
					     	if(t == INT)
								fprintf(asm_file,"movsx rsi,word[%s]\n",name);
							else if(t == BOOL)
								fprintf(asm_file,"movsx rsi,byte[%s]\n",name);
							fprintf(asm_file,"mov al,0\ncall printf\n");
						}
				    }
					else
					{
						fprintf(asm_file,"mov rdi,formatout\n");
						astptr index_node = aroot->children[0]->children[1]->children[0];
						int index_val;
						varptr vt = get_id(aroot->children[0]->children[0]->name.tok.lexeme,broot);
						int index_low = vt->var.type.low;

						if(index_node->children[0]->name.tok.type == NUM)
						{
							index_val = get_int(index_node->children[0]->name.tok.lexeme);
							int offset = (index_val - index_low)*data_bytes[(int)t];
							if(t == INT)
								fprintf(asm_file,"movsx rsi,word[%s+%d]\n",name,offset);
							else if(t == BOOL)
								fprintf(asm_file,"movsx rsi,byte[%s+%d]\n",name,offset);
							fprintf(asm_file,"mov al,0\ncall printf\n");
						}
						else
						{
							char ind_name[50];
							variable_get(ind_name,index_node->children[0]);
							if(t == INT)
								fprintf(asm_file,"movzx rsi,word[%s]\n",ind_name);
							else if(t == BOOL)
								fprintf(asm_file,"movzx rsi,byte[%s]\n",ind_name);
							
							fprintf(asm_file,"sub rsi,%d\n",index_low);
							fprintf(asm_file,"mov rdi,formatout\n");
							if(t == INT)
								fprintf(asm_file,"movsx rsi,word[%s+rsi*%d]\n",name,data_bytes[(int)t]);
							else if(t == BOOL)
								fprintf(asm_file,"movsx rsi,byte[%s+rsi*%d]\n",name,data_bytes[(int)t]);
							fprintf(asm_file,"mov al,0\ncall printf\n");
						}
					}
					
				}
				else if(aroot->children[0]->children[0]->name.tok.type == NUM)
				{
					fprintf(asm_file,"mov rdi,formatout\n");
				    fprintf(asm_file,"mov rsi,%s\n",aroot->children[0]->children[0]->name.tok.lexeme);
				    fprintf(asm_file,"mov al,0\ncall printf\n");
				}
				
			}
}

void check_assignstmt(astptr aroot, bptr broot){
	generate_asm(aroot->children[1],broot);
			char name[50];
			variable_get(name,aroot->children[0]);
			if(aroot->children[1]->children[0]->name.tok.type == lvalueIDStmt)
			{	
				if(aroot->children[0]->type == INT)
				{
					fprintf(asm_file,"mov word[%s],dx\n",name);
				}
				else if(aroot->children[0]->type == RE)
				{
					fprintf(asm_file,"mov dword[%s],edx\n",name);
				}
				else if(aroot->children[0]->type == BOOL)
				{
					fprintf(asm_file,"mov byte[%s],dl\n",name);
				}
		    }
		    else
		    {
		    	astptr index_node = aroot->children[1]->children[0]->children[0];
				int index_val;
				varptr vt = get_id(aroot->children[0]->name.tok.lexeme,broot);
				typ t = aroot->children[0]->type;
				int index_low = vt->var.type.low;
				index_val = get_int(index_node->children[0]->name.tok.lexeme);
				int offset = (index_val - index_low)*data_bytes[(int)t];
		    	if(index_node->children[0]->name.tok.type == NUM)
		    	{
			    	if(aroot->children[0]->type == INT)
					{
						fprintf(asm_file,"mov word[%s+%d],dx\n",name,offset);
					}
					else if(aroot->children[0]->type == RE)
					{
						fprintf(asm_file,"mov dword[%s+%d],edx\n",name,offset);
					}
					else if(aroot->children[0]->type == BOOL)
					{
						fprintf(asm_file,"mov byte[%s+%d],dl\n",name,offset);
					}
			    }
			    else
			    {
			    	// array index is a variable
			    	char ind_name[50];
					variable_get(ind_name,index_node->children[0]);
					if(t == INT)
						fprintf(asm_file,"movzx rsi,word[%s]\n",ind_name);
					else if(t == BOOL)
						fprintf(asm_file,"movzx rsi,byte[%s]\n",ind_name);
				
					fprintf(asm_file,"sub rsi,%d\n",index_low);
					if(aroot->children[0]->type == INT)
					{
						fprintf(asm_file,"mov word[%s+rsi*%d],dx\n",name,data_bytes[(int)t]);
					}
					else if(aroot->children[0]->type == RE)
					{
						fprintf(asm_file,"mov dword[%s+rsi*%d],edx\n",name,data_bytes[(int)t]);
					}
					else if(aroot->children[0]->type == BOOL)
					{
						fprintf(asm_file,"mov byte[%s+rsi*%d],dl\n",name,data_bytes[(int)t]);
					}
			    }
		    }
}

void check_iterativestmt(astptr aroot, bptr broot){
	if(aroot->children[0]->name.tok.type == FOR)
			{
				char name[50];
				variable_get(name,aroot->children[1]);
				fprintf(asm_file,"mov word[%s],%s\n",name,aroot->children[2]->children[0]->name.tok.lexeme);
				int curr_marker = marker;
				fprintf(asm_file,"cmp word[%s],%s\njg L%d\nL%d:\n",name,aroot->children[2]->children[1]->name.tok.lexeme,curr_marker+1,curr_marker);
				marker+=2;
				generate_asm(aroot->children[4],broot);
				fprintf(asm_file,"inc word[%s]\n",name);
				fprintf(asm_file,"cmp word[%s],%s\njle L%d\nL%d:\n",name,aroot->children[2]->children[1]->name.tok.lexeme,curr_marker,curr_marker+1);
			}
			else
			{
				generate_asm(aroot->children[1],broot);
				int curr_marker = marker;
				fprintf(asm_file,"cmp dl,0\nje L%d\nL%d:\n",curr_marker+1,curr_marker);
				marker+=2;
				generate_asm(aroot->children[3],broot);
				generate_asm(aroot->children[1],broot);
				fprintf(asm_file,"cmp dl,1\nje L%d\nL%d:\n",curr_marker,curr_marker+1);
			}
}

void checkcase_stmt(astptr aroot, bptr broot){
	char name[50];
			get_switch_id(name,aroot);
			typ sw_type;
			if(aroot->children[0]->children[0]->name.tok.type != NUM)
				sw_type = BOOL;
			else
				sw_type = INT;
		
			if(sw_type == INT)
				fprintf(asm_file,"cmp word[%s],%s\n",name,aroot->children[0]->children[0]->name.tok.lexeme);
			else
			{
				if(aroot->children[0]->children[0]->name.tok.type == TRUE)
					fprintf(asm_file,"cmp byte[%s],1\n",name);
				else
					fprintf(asm_file,"cmp byte[%s],0\n",name);
			}
			int curr_marker = marker;
			fprintf(asm_file,"jne L%d\n",curr_marker);
			marker++;
			generate_asm(aroot->children[1],broot);
			fprintf(asm_file,"jmp L%d\n",get_switch_end(aroot));
			fprintf(asm_file,"L%d:\n",curr_marker);
			generate_asm(aroot->children[2],broot);
}

void check_exp(astptr aroot, bptr broot){
	if(aroot->no_of_children == 1 && aroot->children[0]->name.c == arithmeticOrBooleanExpr)
			{
				generate_asm(aroot->children[0],broot);
			}
			else if(aroot->children[0]->name.tok.type == PLUS)
			{
				generate_asm(aroot->children[1],broot);
			}
			else
			{
				generate_asm(aroot->children[1],broot);
			}
}

void check_abexp(astptr aroot, bptr broot){
	generate_asm(aroot->children[0],broot);
			if(aroot->children[1]->no_of_children > 1)
			{
				l_traverse(aroot->children[0]);
				generate_asm(aroot->children[1],broot);
				operations(aroot->children[1]->children[0]->children[0],aroot->children[0]->type);
			}
}

void check_arexp(astptr aroot,bptr broot){
				generate_asm(aroot->children[0],broot);
			if(aroot->children[1]->no_of_children > 1)
			{
				l_traverse(aroot->children[0]);
				generate_asm(aroot->children[1],broot);
				//printf("%s\n",aroot->children[1]->children[0]->children[0]->name.tok.lexeme);
				operations(aroot->children[1]->children[0]->children[0],aroot->children[0]->type);
			}
}

void check_var(astptr aroot, bptr broot){
	if(aroot->children[0]->name.tok.type == NUM)
			{
				fprintf(asm_file,"mov dx,%s\n",aroot->children[0]->name.tok.lexeme);
			}
			else 
			{
				char name[50];
				variable_get(name,aroot->children[0]);
				typ t = aroot->children[0]->type;
				if(aroot->children[1]->children[0]->is_terminal)
				{
					if(t == INT)
						fprintf(asm_file,"mov dx,[%s]\n",name);
					else if(t == BOOL)
						fprintf(asm_file,"mov dl,[%s]\n",name);
				}
				else
				{
				    astptr index_node = aroot->children[1]->children[0];					
					varptr vt = get_id(aroot->children[0]->name.tok.lexeme,broot);
					int index_low = vt->var.type.low;
					int index_val;
					
					if(index_node->children[0]->name.tok.type == NUM)
					{
						index_val = get_int(index_node->children[0]->name.tok.lexeme);
						int offset = (index_val - index_low)*data_bytes[(int)t];
						if(t == INT)
							fprintf(asm_file,"mov dx,[%s+%d]\n",name,offset);
						else if(t == BOOL)
							fprintf(asm_file,"mov dl,[%s+%d]\n",name,offset);
					}
					else
					{
						char ind_name[50];
						variable_get(ind_name,index_node->children[0]);
						if(t == INT)
							fprintf(asm_file,"movzx rsi,word[%s]\n",ind_name);
						else if(t == BOOL)
							fprintf(asm_file,"movzx rsi,byte[%s]\n",ind_name);
						fprintf(asm_file,"sub rsi,%d\n",index_low);

						if(aroot->children[0]->type == INT)
						{
							fprintf(asm_file,"mov dx,[%s+rsi*%d]\n",name,data_bytes[(int)t]);
						}
						else if(aroot->children[0]->type == RE)
						{
							fprintf(asm_file,"mov edx,[%s+rsi*%d]\n",name,data_bytes[(int)t]);
						}
						else if(aroot->children[0]->type == BOOL)
						{
							fprintf(asm_file,"mov dl,[%s+rsi*%d]\n",name,data_bytes[(int)t]);
						}
					}
				}
			}
}

void generate_asm(astptr aroot , bptr broot)
{
	if(!aroot->is_terminal)
	{

		switch(aroot->name.c){
			case program:
				check_pro(aroot, broot);
				break;
			case driverModule:
				generate_asm(aroot->children[0],broot);
				break;
			case moduleDef:
				generate_asm(aroot->children[1],broot->children[0]);
				break;
			case statements:
				if(aroot->children[0]->is_terminal) return;
				generate_asm(aroot->children[0],broot);
				generate_asm(aroot->children[1],broot);
				break;
			case statement:
				generate_asm(aroot->children[0],broot);
				break;
			case ioStmt:
				check_iostmt(aroot,broot);
				break;
			case simpleStmt:
				generate_asm(aroot->children[0],broot);
				break;
			case assignmentStmt:
				check_assignstmt(aroot, broot);
				break;
			case iterativeStmt:
				check_iterativestmt(aroot,broot);
				break;
			case conditionalStmt:
				aroot->end = marker;
				marker++;
				generate_asm(aroot->children[2],broot);
				generate_asm(aroot->children[3],broot);
				fprintf(asm_file,"L%d:\n",aroot->end);
				break;
			case caseStmts:
				checkcase_stmt(aroot,broot);
				break;
			case var:
				check_var(aroot,broot);
				break;
			case N9:
				if(aroot->no_of_children == 1) return;
				char name[50];
				get_switch_id(name,aroot);
				typ sw_type;
				if(aroot->children[0]->children[0]->name.tok.type != NUM)
				sw_type = BOOL;
				else
					sw_type = INT;
		
				if(sw_type == INT)
					fprintf(asm_file,"cmp word[%s],%s\n",name,aroot->children[0]->children[0]->name.tok.lexeme);
				else
				{
					if(aroot->children[0]->children[0]->name.tok.type == TRUE)
						fprintf(asm_file,"cmp byte[%s],1\n",name);
					else
						fprintf(asm_file,"cmp byte[%s],0\n",name);
				}	
				int curr_marker = marker;
				fprintf(asm_file,"jne L%d\n",curr_marker);
				marker++;
				generate_asm(aroot->children[1],broot);
				fprintf(asm_file,"jmp L%d\n",get_switch_end(aroot));
				fprintf(asm_file,"L%d:\n",curr_marker);
				generate_asm(aroot->children[2],broot);
				break;
			case N8:
				generate_asm(aroot->children[1],broot);
				if(aroot->children[2]->no_of_children > 1)
				{
					l_traverse(aroot->children[1]);
					generate_asm(aroot->children[2],broot);
					operations(aroot->children[2]->children[0]->children[0],aroot->children[1]->type);
				}
				break;
			case N7:
				generate_asm(aroot->children[1],broot);
				if(aroot->children[2]->no_of_children > 1)
				{
					l_traverse(aroot->children[1]);
					generate_asm(aroot->children[2],broot);
					operations(aroot->children[2]->children[0]->children[0],aroot->children[1]->type);
				}
				break;
			case N5:
				generate_asm(aroot->children[1],broot);
				if(aroot->children[2]->no_of_children > 1)
				{
					l_traverse(aroot->children[1]);
					generate_asm(aroot->children[2],broot);
					operations(aroot->children[2]->children[0]->children[0],aroot->children[1]->type);
				}
				break;
			case N4:
				generate_asm(aroot->children[1],broot);
				if(aroot->children[2]->no_of_children > 1)
				{
					l_traverse(aroot->children[1]);
					generate_asm(aroot->children[2],broot);
					operations(aroot->children[2]->children[0]->children[0],aroot->children[1]->type);
				}
				break;
			case factor:
				generate_asm(aroot->children[0],broot);
				break;
			case Default:
				if(aroot->children[0]->name.tok.type == eps) return;
				generate_asm(aroot->children[0],broot);
				break;
			case whichStmt:
				if(aroot->children[0]->name.c == lvalueIDStmt)
					generate_asm(aroot->children[0]->children[0], broot);
				else
				{
					generate_asm(aroot->children[0]->children[1],broot);
				}
				break;
			case expression:
				check_exp(aroot,broot);
				break;
			case arithmeticOrBooleanExpr:
				check_abexp(aroot,broot);
				break;
			case arithmeticExpr:
				check_arexp(aroot,broot);
				break;
			case AnyTerm:
				generate_asm(aroot->children[0],broot);
				if(aroot->children[1]->no_of_children > 1)
				{
					l_traverse(aroot->children[0]);
					generate_asm(aroot->children[1],broot);
					operations(aroot->children[1]->children[0]->children[0],aroot->children[0]->type);
				}
				break;
			case term:
				generate_asm(aroot->children[0],broot);
				if(aroot->children[1]->no_of_children > 1)
				{
					l_traverse(aroot->children[0]);
					generate_asm(aroot->children[1],broot);
					operations(aroot->children[1]->children[0]->children[0],aroot->children[0]->type);
				}
				break;
		}
}