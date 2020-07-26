/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"


#define Array_Max_Len 300
int offset=0; // for setting the offset of variables 
// int size_track_of_arr_variables = 50;
// int index_track_of_arr_variables = 0;
// ASTNode** track_of_arr_variables; //= (VariableEntry*)malloc(sizeof(VariableEntry) * size_track_of_arr_variables );
/////////////////////////////////////////////////////////////////////////////////////////////////
void symbolMaster(FILE* fp, token_store_node* p)
{	variableTable* VST = initialize_var_ST();
	functionTable* FST = initialize_func_ST();
	 populateSymbolTable(p, VST, FST, fp);
	 printf("Displaying Symbol Tables\n"); 
    printSymbolTables(VST, FST);
    printf("\n");
}
void pushonstack(int data, stack_node2* stack_top)
{
	stack_node2* temp;
	temp = (stack_node2*)malloc(sizeof(stack_node2));
	temp->data = data;
	temp->next = stack_top->next;
	stack_top->next = temp;
	stack_top->depth++;
	return;
}


int popfromstack(stack_node2* top)
{
	if(top->next)
		
	{int x = top->next->data;
	stack_node2* f = top->next;
	top->next = top->next->next;
	top->depth--;
	return x;}
	return -1;
}
int top2(stack_node2* top)
{
	if(top->next)
	{
		return top->next->data;
}		
	
	return -1;
}
stack_node2* new_stack()
{
	stack_node2* a = (stack_node2*)malloc(sizeof(stack_node2));
	a->next = NULL;
	a->data = 0;
	a->depth = 0;
	return a;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

int Hash(char* str)
{
	int h=0;
	//int i=0;
	//printf("%d\n",sizeof(ch)/sizeof(char));
	int len = strlen(str);
	for(int i=0;i< len;i++)
	{   if(str[i]!='\0')
			h=h + (int)str[i];
		else
			break;
	}
	return h%619;
}



void insert_new_variable(variableTable* table,char* parent_func, token_store_node* current, char* type, int top2,int arr_or_var,int start,int end, int nestinglevel, FILE * fp)
{
	// always check for re-declaration
	// hash the value
	
	int pos = Hash(current->lexeme);
	VariableEntry* entry = (VariableEntry*)malloc(sizeof(VariableEntry));
	strcpy(entry->name,current->lexeme);
	entry->pos = pos;
	entry->scoperange.start= top2;
	entry->scoperange.end=-1; 	// end_scope not know yet
	entry->line_number=current->line_numberber;
//	entry->next=NULL;
	entry->depth = nestinglevel;
	strcpy(entry->parent_func,parent_func);


	enum var_type t;
	int type_size =0;
	if(strcmp(type, "integer")==0)
		{t = _INTEGER;
		type_size =2;}
	else if(strcmp(type,"real")==0)
		{t = _REAL; 
		type_size =4;}
	else 
		{ t = _BOOLEAN; // boolean
		  type_size =1;}
	entry->if_array = arr_or_var; 			//done -> // *** CAUTION *** to be changed later // ^^^^^^^^^^^^^^^^^^^^^^^

	switch(entry->if_array)
	{	case 0:
		entry->start_index = 0;
		entry->end_index = 0;
		break;
		case 1:
		entry->start_index = start;
		entry->end_index = end;
	}
	entry->width = (entry->end_index - entry->start_index + 1)*type_size;
	entry->offset = offset;
	offset=offset + entry->width;
	if(table->entries[key]==NULL)
	{
		table->entries[key] = entry;	
	}
	else
	{	int index = key;
		
		while( table->entries[index])
		{
			if(strcmp( table->entries[index], current->lexeme)==0)
			{
				if( table->entries[index]->scope.start== top2)	
				{
					fprintf(fp, "ERROR: RE-DECLARATION of VARIABLE \"%s\" line number %d. \n", current->lexeme, entry->line_number);
					return;	
				}
			} 
			(index++)%619;
		}
		// if it is not found in the list... add in the front of the list
		
		VariableEntry* temp = table->entries[key];
				table->entries[key] = entry;
				table->entries[index] = temp;

		// printf("\t\t\t>>>>>>adding %s at key %d \n", temp->name,temp->key);

	}
/*	VariableEntry* temp0 = table->entries[key];
	while( temp0!= NULL)
		// {printf(" >>>>>>>inserting\n%s\t%d",temp0->name, temp0->scope.start);
		{ 
			temp0=temp0->next;
		}*/
}

void insert_new_Function(functionTable* table, token_store_node* current,int is_defined, int top2, FILE* fp)
{
	// always check for re-declaration
	// hash the value
	// **DOUBT** is declaration possible for "driver program"? driver program should be declared only once

	int key = Hash(current->lexeme);
	
	newFunction* entry = (newFunction*)malloc(sizeof(newFunction));
	entry->is_defined=is_defined;
	strcpy(entry->name, current->lexeme);
	entry->key = key;
	entry->num_output_params=0;
	entry->scoperange.start= top2;
	entry->scoperange.end=-1; 	
	
	
	entry->num_input_params = 0;
	
	/*entry->input_plist = NULL;
	entry->output_plist= NULL;
	entry->next=NULL;*/
	entry->line_number=current->line_number;
	if(table->entries[key]==NULL)
	{
		table->entries[key] = entry;
		// printf("\t\t\t>>>>>>adding %s at key %d \n", entry->name,entry->key);
	}
	else
	{	int index = key;
			// when collision occurs in hashing
		while(table->entries[index]!=NULL)
		{
			if(strcmp(table->entries[index]->name ,current->lexeme)==0)
			{
				if(table->entries[index]->is_defined==1 || table->entries[index]->is_defined == is_defined)	
					fprintf(fp, "ERROR: RE-DEFINITION of FUNCTION \"%s\" line number %d. \n",current->lexeme, entry->line_number);
				
				else
				{
					table->entries[index]->scoperange.end=top2;// will help to mark the end scope of a declared function.
					table->entries[index]->is_defined = 1; // status changed from declaration to definition
				}
				free(entry);
				return;
			} 
			(index++)%619;
		}
		// if it is not found in the list... add in the front of the list
				newFunction* temp = table->entries[key];
				table->entries[key] = entry;
				table->entries[index] = temp;
		// printf("\t\t\t>>>>>>adding %s at key %d \n", temp->name,temp->key);

	}
	//take the scope from the stack_top
	//if it is already declared, check is_defined. 
	// 			if is_defined==0, no Error, make is_defined=1; 
}

variableTable* initialize_var_ST()
{
	variableTable* table = (variableTable*)malloc(sizeof(variableTable));
	table->entries = (VariableEntry**)malloc(619*sizeof(VariableEntry*));
	int i;
	for(i=0; i<619; i++)
	{
		table->entries[i] = NULL;//(VariableEntry*)malloc(sizeof(VariableEntry));
	}
	return table;
}

functionTable* initialize_func_ST()
{
	functionTable* st = (functionTable*)malloc(sizeof(functionTable));
	st->entries = (newFunction**)malloc(619*sizeof(newFunction*));
	int i;
	for(i=0; i<619; i++)
	{
		st->entries[i] = NULL;// (newFunction*)malloc(sizeof(newFunction));
	}
	return st;
}


void update_func_entry( variableTable* VT, functionTable* FT, char* Identifier_parent_function, token_store_node* current, int flag)
{
	int key = Hash(Identifier_parent_function);
	newFunction* temp = FT->entries[key];
	plist* temp2 ;
	plist* new_node = (plist*)malloc(sizeof(plist));
	new_node->v_ptr = VT->entries[Hash(current->lexeme)];
	new_node->next = NULL;
	//if(flag == 1)
		switch (flag)
	{ case 1:
		temp2 = temp->input_plist;
		temp->num_input_params = temp->num_input_params + 1;
		break;
	  default:
	
		temp2 = temp->output_plist;
		temp->num_output_params = temp->num_output_params + 1;
	}
	int index = key;
	// printf("cheeeck %s  %s \n",Identifier_parent_function, temp->name );
	while(temp!=NULL)
	{
		if(strcmp(Identifier_parent_function, temp->name)==0)
		{
			
			// temp2 = temp->input_plist;
			// temp->num_input_params = temp->num_input_params + 1;
			if(!temp2 )
			{
				switch (flag){
					case 1:
					temp->input_plist = new_node;	
					break;
					default:
					temp->output_plist = new_node;
			}
			else
			{
				while(temp2->next)
					temp2 = temp2->next;
				temp2->next = new_node;
			}
			break;
		}
		temp=FT->entries[++index];
	}
}

void PopulateSymbols(token_store_node* list, variableTable* VT, functionTable* FT, FILE* fp)
{
	// variableTable* VT = initialize_var_ST(); 
	// functionTable* FT = initialize_func_ST();
	token_store_node* current = list;
	stack_node2* scoper = new_stack();
	// pushonstack(1,scoper); // the starting line of the code

	int Variables_Till_Now[Array_Max_Len];
	int freeVarTrackindex =0;
	int freeFuncTrackindex =0;
	int Functions_Till_Now[Array_Max_Len];
	int i=0;
	int nestinglevel=-2;
	
	token_store_node* temp;
	while(current)
	{	switch (Hash(current->lexeme))
		{
			case 101:
			{
				if(strcmp(current->next->lexeme,"module")==0)// declaration(not defi.) of a function.
				{
					// printf("Declare Module --\n");
					current=current->next->next; // points to name of function now
					insert_new_Function(FT, current, 0, current->line_number, fp); // 0=> declaration
					
					Functions_Till_Now[freeFuncTrackindex] = Hash(current->lexeme);
					freeFuncTrackindex++;
					// printArray(Functions_Till_Now);
					// current = current->next; // points to ";"
				}
				
				else if(strcmp(current->next->token, "ID")==0)
				{
					current=current->next;	// points to the variable name
					temp=current->next;		// either points to "," or ":"

					char Identifier_parent_function[30];
					if(!freeFuncTrackindex>0 && !freeFuncTrackindex<0)
						strcpy(Identifier_parent_function,"--");
					else
					{

						strcpy(Identifier_parent_function, FT->entries[Functions_Till_Now[freeFuncTrackindex-1]]->name);
					}	

					while(strcmp(current->lexeme,";")!=0)
					{
						// printf("here3\n");
						while(strcmp(temp->lexeme,":")!=0)
							temp=temp->next;
						// printf("c: %s\n",current->lexeme );
						// printf("t: %s\n",temp->lexeme );

						while(current!=temp)
						{
							//add all the vars before ":" to the table
							// printf("here4\n");
							if(strcmp(current->lexeme,",")==0)
							{
								current=current->next;
								continue;
							}

							if(strcmp(temp->next->lexeme,"array")==0)
							{
								// current points to var name, temp points to ":"
								//var_name : use "current" node only
								token_store_node* temp2 = temp->next->next->next; // points to 1..12
								int s_index = atoi(temp2->lexeme);
								int e_index = atoi(temp2->next->next->lexeme);
								
								token_store_node* array_type = temp2->next->next->next->next->next;
								nestinglevel = (scoper->depth);
								//				table,var_node, 	var_type		, scope  		, if_array,start_in, end_ind, ar_type
								insert_new_variable(VT,Identifier_parent_function,current,array_type->lexeme,top2(scoper), 1 ,s_index, e_index  ,nestinglevel, fp); // temp->lexeme gives type:"integer"/ "boolean"/ "real" 
								
								int key=Hash(current->lexeme);
								Variables_Till_Now[freeVarTrackindex] = key;
								freeVarTrackindex++;
								if(strcmp(current->next->lexeme,",")==0)
									current = current->next;
								else// it points to ":"
								{
									if(strcmp(array_type->next->lexeme, ",")==0)
									{
										current = array_type->next->next;	// both point to a new var
										temp = current;
									}
									else // it is ";"
									{

										current = array_type->next;	// both point to ";"
										temp = current;
									}
									
								}


							}
							else
							{	// deal with all other types
								nestinglevel = (scoper->depth);
								insert_new_variable(VT,Identifier_parent_function,current,temp->next->lexeme,top2(scoper),0,0,0, nestinglevel, fp); // temp->lexeme gives type:"integer"/ "boolean"/ "real" 
								
								Variables_Till_Now[freeVarTrackindex] = Hash(current->lexeme);
								freeVarTrackindex++;
								if(strcmp(current->next->lexeme,",")==0)
									current = current->next;
								else //if current->next points to ':'
									{current = current->next->next->next;
										temp= current;
									}
							}
				 		}
					}
					// printf("exiting declare \n");
				}
			}
			break;
			case 27:
			{
				// printf("Module --\n");
				temp =current;
				while(strcmp(temp->lexeme,"start")!=0)	// to find the start_scope of variable
					temp=temp->next;
				int line_number = temp->line_number;
				pushonstack(line_number,scoper);
				// printf("pushed %s at stack@@@@@@@%d\n",current->next->lexeme,line_number);
				insert_new_Function(FT, current->next, 1, top2(scoper), fp); // 0=> declaration; 1=>definition
				
				Functions_Till_Now[freeFuncTrackindex] = Hash(current->next->lexeme);
				freeFuncTrackindex++;
				// printArray(Functions_Till_Now);
				current=current->next->next;

			}
			break;
			
			case 33:
			{
				// printf("Driver --\n");
				current=current->next;
				temp=current;
				while(strcmp(temp->lexeme,"start")!=0)	// to find the start_scope of variable
					temp=temp->next;
				int line_number = temp->line_number;
				pushonstack(line_number,scoper);
				insert_new_Function(FT, current, 1, top2(scoper), fp); // 0=> declaration
				
				Functions_Till_Now[freeFuncTrackindex] = Hash(current->lexeme);
				freeFuncTrackindex++;
				// printArray(Functions_Till_Now);

			}
			break;
			case 333:
			{
				// printf("Use --\n");
				current=current->next->next; // points to the called module(function) (NAME)
				int key = Hash(current->lexeme);
				newFunction* check_func = FT->entries[key];
				int ind = key;
				while(check_func != NULL)
				{
					if((strcmp(check_func->name, current->lexeme)==0))
					{
						break;
					}				
					check_func = FT->entries[++ind];
				}

				if(check_func == NULL)  //neither declared nor defined // but for now, we are checking declaration only.
				{
					fprintf(fp, "ERROR: Unknown Function \"%s\" line number %d.\n", current->lexeme, current->line_number);
				}
				// checking recursive fuction.
				temp = current->next->next->next;
				VariableEntry* check_parameter = VT->entries[Hash(temp->lexeme)];
				int index = Hash(temp->lexeme);
				while (check_parameter!=NULL)
				{
					if(strcmp(check_parameter->name,temp->lexeme)==0)
					{
						if(check_parameter->scope.end == -1) // the parameter variable is of the same function
						{
							if(strcmp(check_parameter->parent_func,current->lexeme)==0)
							{
								fprintf(fp, "ERROR: Recursion of the \"%s\" line %d \n", check_parameter->parent_func,current->line_number);
								break;
							}
						}
					}
					check_parameter=VT->entries[++index];
				}
					
			}
			break;
			case 558:
			{
				// printf("Start --\n");
				int line_number = current->line_number;	// ** DOUBT ** add line_number+1 ??
				if(top2(scoper) != line_number)	// avoid double pushonstacking of "start"; already pushonstacked in case of: "takes input" [a:boolean...]; "returns" [sum:integer]
					pushonstack(line_number,scoper);
			}
			break;
			case 536: //take input [a:integer, b:real];
			{
				
				current=current->next->next->next;	// points to the variable name
				temp=current->next;		// either points to "," or ":"
				int x=0;
				while(Functions_Till_Now[x]!=-1)
					x++;
				x=Functions_Till_Now[x-1];
				char Identifier_parent_function[30];		
				if(x<0)LE
					strcpy(Identifier_parent_function,"--");
				else
				{
					// printf("x: %d func:%s check parent name: %s\n",x,current->lexeme,FT->entries[x]->name );
					strcpy(Identifier_parent_function, FT->entries[x]->name);
				}	
				
				while(strcmp(current->lexeme,"]")!=0)
				{
					while(strcmp(temp->lexeme,":")!=0)
						temp=temp->next;
					
					while(current!=temp)
					{
						//add all the vars before ":" to the table
						if(strcmp(current->lexeme,",")==0)
						{
							current=current->next;
							continue;
						}
						if(strcmp(temp->next->lexeme,"array")==0)
						{
							// current points to var name, temp points to ":"
							//var_name : use "current" node only
							token_store_node* temp2 = temp->next->next->next; // points to 1..12
								int s_index = atoi(temp2->lexeme);
								int e_index = atoi(temp2->next->next->lexeme);
								
								token_store_node* array_type = temp2->next->next->next->next->next;
								nestinglevel = (scoper->depth);
							//				table,              var_node, 	var_type		, scope  		, if_array,start_in, end_ind, ar_type
							insert_new_variable(VT,Identifier_parent_function,current,array_type->lexeme,top2(scoper), 1 ,s_index, e_index  ,nestinglevel, fp); // temp->lexeme gives type:"integer"/ "boolean"/ "real" 
							update_func_entry(VT, FT, Identifier_parent_function, current,1); // 1 signifies input_list
							
							Variables_Till_Now[freeVarTrackindex] = Hash(current->lexeme);
							freeVarTrackindex++;
							if(strcmp(current->next->lexeme,",")==0)
								current = current->next;
							else// it points to ":"
							{
								if(strcmp(array_type->next->lexeme, ",")==0)
								{
									current = array_type->next->next;	// both point to a new var
									temp = current;
								}
								else // it is ";"
								{
									current = array_type->next;	// both point to ";"
									temp = current;
								}							
							}
						}
						else
						{	// deal with all other types
							nestinglevel = (scoper->depth);
							insert_new_variable(VT,Identifier_parent_function,current,temp->next->lexeme,top2(scoper),0,0,0,nestinglevel,fp); // temp->lexeme gives type:"integer"/ "boolean"/ "real" 
							update_func_entry(VT, FT, Identifier_parent_function, current,1);
							
							Variables_Till_Now[freeVarTrackindex] = Hash(current->lexeme);
							freeVarTrackindex++;
							if(strcmp(current->next->lexeme,",")==0)
								current = current->next;
							else //if current->next points to ':'
								{current = current->next->next->next;
									temp= current;
								}
						}
					}
				}
			}
			break;
			case 136:
			{
				// printf("Returns --\n");
				current=current->next->next; // points to the variable
				int x=0;
				while(Functions_Till_Now[x]!=-1)
					x++;
				x=Functions_Till_Now[x-1];
				char Identifier_parent_function[30];
				if(x<0)
					strcpy(Identifier_parent_function,"--");
				else
				{
					strcpy(Identifier_parent_function, FT->entries[x]->name);
				}	

				while((current->lexeme)[0]!=']')
				{
					if((current->lexeme)[0]== ',')
					{
						current=current->next;
						continue;
					}
					nestinglevel = (scoper->depth);
					insert_new_variable(VT, Identifier_parent_function,current, current->next->next->lexeme,top2(scoper),0,0,0,nestinglevel,fp);
					update_func_entry(VT, FT, Identifier_parent_function, current,0); // 0 signifies output list, 1 signifies input_list
					
					Variables_Till_Now[++freeVarTrackindex] = Hash(current->lexeme);

					current = current->next->next->next; // "," or "]"
				}
				// current=current->next; // points to ";"
			}
			break;
			case 311:
			{
				// printf("End --\n");
				int end_scope = current->line_number;
				int start_scope = top2(scoper);
				int i=0;
				// printf("closing scope of %d %d \n", start_scope,end_scope);
				// printf("printing before Variables_Till_Now\n");
				int t=0;
				// for(t=0;t<Array_Max_Len/10;t++)
				// 	printf("%d ",Variables_Till_Now[t] );
				// printf("\n");
				// adding end_scope in VT
				i = 0;
				while(Variables_Till_Now[i]!=-1)
				{
					i++;
				}
				i = freeVarTrackindex-1;
				// printf("\niiiiiiiiiiii %d\n",i);
				VariableEntry* temp1;

				while(i >= 0)
				{
					temp1 = VT->entries[Variables_Till_Now[i]];
					index = Variables_Till_Now[i];
					while(temp1!= NULL)
					{	
						// printf("@@@@ %s scope entry: %d, start_scope: %d  \n",temp1->name,temp1->scope.start,start_scope);
						if(temp1->scope.start == start_scope && temp1->scope.end == -1)
						{

							temp1->scope.end = end_scope;
							Variables_Till_Now[i] = -1;
							freeVarTrackindex = i;
						}
						temp1 = VT->entries[++index];
					} 
					i=i-1;
				}
				// printf("printing after Variables_Till_Now\n");
				// for(t=0;t<Array_Max_Len/10;t++)
				// 	printf("%d ",Variables_Till_Now[t] );
				i=0;
				while(Functions_Till_Now[i]!=-1)
				{
					if(FT->entries[Functions_Till_Now[i]]->scope.end == start_scope)	// handling a declared function
					{
						FT->entries[Functions_Till_Now[i]]->scope.end = end_scope;
						Functions_Till_Now[i] = -1;
						Functions_Till_Now[i+1] = -1;
						i++;

					}
					else if(FT->entries[Functions_Till_Now[i]]->scope.start == start_scope)
					{
						FT->entries[Functions_Till_Now[i]]->scope.end = end_scope;
						Functions_Till_Now[i] = -1;
					}
					i=i+1; 
				}
				popfromstack(scoper);
				if(top2(scoper)==-1)
				{
					offset = 0;
				}
			}
		}

		switch (strcmp(current->token,"ID"))
		{
			case 0:
		{


			int key = Hash(current->lexeme);
			index = key;
			VariableEntry* check_var = VT->entries[key];
			
			while(check_var )
			{

				if((strcmp(check_var->name, current->lexeme)==0))
				{
					// printf("lexeme:%s stack top %d ; current scope %d to %d; line_number: %d\n",  current->lexeme, top2(scoper), check_var->scope.start, check_var->scope.end, current->line_number);
					if((check_var->scope.start < current->line_number) && (check_var->scope.end == -1))
					{
						 break;	
					}

					else
					{
						fprintf(fp, "ERROR: Use Variable \"%s\" out of scope line number %d.\n", current->lexeme, current->line_number);
						break;
					}
				}
				check_var = VT->entries[(++index)%619];
			}
			if(check_var == NULL)
			{
				fprintf(fp, "ERROR: Use  Variable \"%s\" without DECLARATION  line number %d.\n", current->lexeme, current->line_number);
			}

		}
		break;
		}
		
		current=current->next;
	}
	fclose(fp);
	return;
}

void printSymbolTables(variableTable* VT, functionTable* FT)
{
	VariableEntry* temp2;
	VariableEntry* travel_ptr;
	int s_no = 1;
	int n = 20, n1 = 6;
	printf("******************************* Variable Symbol Table************************************\n\n");
	printf("\nSNo.\t\tLexeme\t\tVarType\t\t\tscope\t\tscope\t\tscope\t\tWidth\t\tOffset\n");
	printf("\t\t\t\t\t\t\t(parent func)\t(line numbers)\t(nestinglevel level)\t\t\n");
	int i;
	for(i=0;i<619;i++)
	{
		travel_ptr=VT->entries[i];
		if(VT->entries[i])
		{
			temp2 = travel_ptr;
			printf("%*d ", n1, s_no++);
			printf("%*s ", n, temp2->name);
				
			if(temp2->if_array)
			{
				enum var_type arr_type = temp2->type;
				int start_index = temp2->start_index;
				int end_index = temp2->end_index;
				int arr_size = end_index-start_index+1;
				// printf("%d ", start_index);
				// printf("%d ", end_index);
				printf("\t  array(%*s,%d)", 4, var_map[arr_type], arr_size);
				printf("%*s", n, temp2->parent_func);
				printf("%*d to %d", n1, temp2->scope.start, temp2->scope.end);
				printf("%*d", 10, temp2->depth);	
				printf("%*d", n1*3, temp2->width);
				printf("%*d ", n1*2, temp2->offset);

			}	
			else
			{   //print for var type
				enum var_type type = temp2->type;
				printf("%*s", n, var_map[type]);
				printf("%*s", n, temp2->parent_func);
				printf("%*d to %d", n1, temp2->scope.start, temp2->scope.end);
				printf("%*d", 10, temp2->depth);	
				printf("%*d", n1*3, temp2->width);
				printf("%*d", n1*2, temp2->offset);
				//print var_width		
			}		
			//travel_ptr=travel_ptr->next;
			printf("\n");	
		}
	}
	printf("******************************* Function Symbol Table************************************\n\n");
	plist* in_plist_ptr;
	plist* out_plist_ptr;
	newFunction* FT->entries[i];
	int sno = 0;
	printf("SNo.\t\tlexeme\t\tscope\tinput_params\toutput_params\n");
	printf("\t\t(func id)\t\t\t\t(line numbers)\n");
	for(i=0;i<619;i++)
	{

		while(FT->entries[i]!=NULL)
		{
			printf("%*d", n1, sno++);
			printf("%*s %*d to %d", n, FT->entries[i]->name, n1, FT->entries[i]->scope.start, FT->entries[i]->scope.end);
			in_plist_ptr = FT->entries[i]->input_plist;
			out_plist_ptr= FT->entries[i]->output_plist;
			printf("\t |%d| ",FT->entries[i]->num_input_params);
			while(in_plist_ptr!=NULL)
			{
				printf("%s ", in_plist_ptr->v_ptr->name);
				in_plist_ptr= in_plist_ptr->next;
			}
			printf(" \t|%d| ",FT->entries[i]->num_output_params);
			while(out_plist_ptr!=NULL)
			{
				printf("%s ", out_plist_ptr->v_ptr->name);
				out_plist_ptr= out_plist_ptr->next;
			}
			printf("\n" );
			//FT->entries[i] = FT->entries[i]->next;
		}
	}
}
