/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include "symbolTableDef.h"

void pushonstack(int stack_data, stack_node2* stack_top);
int is_stackEmpty2(stack_node2* top2);
int popfromstack(stack_node2* stack_top);
int top2(stack_node2* stack_top);
stack_node2* create_stack2();
int Hash(char* str);
void insert_new_variable(variableTable* table,char* parent_func, token_store_node* current, char* type, int top2,int arr_or_var,int start,int end, int nesting, FILE* fp);
void insert_new_Function(functionTable* table, token_store_node* current,int is_defined, int top2, FILE* fp);
variableTable* initialize_var_ST();
functionTable* initialize_func_ST();
int addtoTrackerArray(int* array);
void PopulateSymbols(token_store_node* list, variableTable* VT, functionTable* FT, FILE* fp);