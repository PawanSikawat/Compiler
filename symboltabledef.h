/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */


#include "lexerDef.h"

#define PRIME_NUMBER 619

enum var_type
{
    _NONE, _INTEGER, _REAL, _BOOLEAN , _MODULE
};

typedef struct VariableEntry VariableEntry;
typedef struct variableTable variableTable;
typedef struct newFunction newFunction;
typedef struct functionTable functionTable;
typedef struct array array;
typedef enum var_type var_type;
typedef struct plist plist;

char* var_map[5] = {"None","Integer", "Real", "Boolean", "module"};
struct plist			// parameters list for the functable entry
{
	VariableEntry* v_ptr;
	struct plist* next;
};
struct array
{
	enum var_type type;
	int width[2];  // width[0] => start index , width[1] => end index
	//final-initial+1
	//a[1..10], 1..10 is a single lexeme
};
typedef struct array array;
struct scope
{
	int start;
	int end;
};
typedef struct scope scope;
struct newFunction
{
	char name[30];  //var name
	int key;
	 scope scoperange; //a[0] -> start of scope; a[1] ->end of scope (line  numbers)
	int is_defined;
	int line_num;

	int num_input_params;
	int num_output_params;
	plist* input_plist;
	plist* output_plist;
//	plisTable* output_plist;

	struct newFunction* next;
}; 

struct functionTable
{
	newFunction** entries;
};


struct VariableEntry
{
	char name[30];  //var name
	int pos;
	scope scoperange; 
	bool if_array; //flag
	int line;
	var_type type; 
	int start_index;
	int end_index;
	//struct VariableEntry* next;
	int offset;
	int width;
	char parent_function[30];
	int depth;
};

struct variableTable
{
	VariableEntry** entries;
};

struct stack_node2
{
	int data;
	struct stack_node2* next;
	int depth;
};
typedef struct stack_node2 stack_node2;