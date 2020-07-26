/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

///////////////////////////////////////////------------------------------ STACK

struct Stack { 
	int top; 
	unsigned capacity; 
	int array[1000]; 
}; 


const int MAX_SIZE_NT = 20;
const int MAX_SIZE_T = 14;
const int GR_RULES = 104;  //////--------------------  Initiallly 103
const int T_COUNT = 58;
const int NT_COUNT = 55;
const int MAX_RHS = 15;
const int MAX_FIRST = 15;
const int MAX_FOLLOW = 25;
const char NT_SYMBOLS[NT_COUNT][MAX_SIZE_NT] = {"program","moduleDeclarations","moduleDeclaration","otherModules","driverModule","module","ret","input_plist","more_input","output_plist",
									 "more_output","dataType","type","moduleDef","statements","statement","ioStmt","print_var","var","whichId","simpleStmt","assignmentStmt","whichStmt",
									 "lvalueIDStmt","lvalueARRStmt",
									 "index","moduleReuseStmt","optional","idList","more_id","expression","U","more_u","arithOrBoolExpr","more_abexp","AnyTerm","more_arith","boolConst","arithmeticExpr","more_term",
									 "term","more_factor","factor","pl_mi","mu_di","logicalOp","relationalOp","declareStmt","conditionalStmt","caseStmt","more_case","value","defaultStmt","iterativeStmt",
									 "range"};
const char T_SYMBOLS[T_COUNT][MAX_SIZE_T] = {"EPSILON","END_OF_STRING","INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", "USE", "WITH",
				"PARAMETERS", "TRUE", "FALSE", "TAKES", "INPUT", "RETURNS", "AND", "OR", "FOR", "IN", "SWITCH", "CASE", "BREAK", "DEFAULT", "WHILE",
				"PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP",
				"SQBO", "SQBC", "BO", "BC", "ID", "DRIVERDEF", "DRIVERENDDEF","NUM", "RNUM"};						
const int grammar[GR_RULES][MAX_RHS]=
{
{80,81,83,84,83,0,0,0,0,0,0,0,0,0,0} ,//0
{81,82,81,0,0,0,0,0,0,0,0,0,0,0,0} ,//1
{81,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//2
{82,10,11,54,47,0,0,0,0,0,0,0,0,0,0} ,//3
{83,85,83,0,0,0,0,0,0,0,0,0,0,0,0} ,//4
{83,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//5
{84,55,12,13,56,93,0,0,0,0,0,0,0,0,0} ,//6
{85,43,11,54,44,21,22,50,87,51,47,86,93,0,0} ,//7
{86,23,50,89,51,47,0,0,0,0,0,0,0,0,0} ,//8
{86,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//9
{87,54,45,91,88,0,0,0,0,0,0,0,0,0,0} ,//10
{88,48,54,45,91,88,0,0,0,0,0,0,0,0,0} ,//11
{88,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//12
{89,54,45,92,90,0,0,0,0,0,0,0,0,0,0} ,//13
{90,48,54,45,92,90,0,0,0,0,0,0,0,0,0} ,//14
{90,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//15
{91,3,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//16
{91,4,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//17
{91,5,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//18
{91,7,50,134,51,6,92,0,0,0,0,0,0,0,0} ,//19
{92,3,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//20
{92,4,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//21
{92,5,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//22
{93,8,94,9,0,0,0,0,0,0,0,0,0,0,0} ,//23
{94,95,94,0,0,0,0,0,0,0,0,0,0,0,0} ,//24
{94,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//25
{95,96,00,0,0,0,0,0,0,0,0,0,0,0,0} ,//26
{95,100,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//27
{95,127,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//28
{95,128,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//29
{95,133,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//30
{96,14,52,54,53,47,0,0,0,0,0,0,0,0,0} ,//31
{96,15,52,97,53,47,0,0,0,0,0,0,0,0,0} ,//32
{97,98,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//33
{97,19,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//34
{97,20,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//35
{98,54,99,0,0,0,0,0,0,0,0,0,0,0,0} ,//36
{98,57,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//37
{98,58,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//38
{99,50,105,51,0,0,0,0,0,0,0,0,0,0,0} ,//39
{99,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//40
{100,101,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//41
{100,106,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//42
{101,54,102,0,0,0,0,0,0,0,0,0,0,0,0} ,//43
{102,103,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//44
{102,104,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//45
{103,49,110,47,0,0,0,0,0,0,0,0,0,0,0} ,//46
{104,50,105,51,49,110,47,0,0,0,0,0,0,0,0} ,//47  ///////// Error Before
{105,57,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//48
{105,54,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//49
{106,107,16,11,54,17,18,108,47,0,0,0,0,0,0} ,//50
{107,50,108,51,49,0,0,0,0,0,0,0,0,0,0} ,//51
{107,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//52
{108,54,109,0,0,0,0,0,0,0,0,0,0,0,0} ,//53
{109,48,54,109,0,0,0,0,0,0,0,0,0,0,0} ,//54
{109,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//55
{110,111,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//56
{110,113,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//57
{111,34,112,0,0,0,0,0,0,0,0,0,0,0,0} ,//58
{111,33,112,0,0,0,0,0,0,0,0,0,0,0,0} ,//59
{112,52,118,53,0,0,0,0,0,0,0,0,0,0,0} ,//60
{112,98,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//61
{113,115,114,0,0,0,0,0,0,0,0,0,0,0,0} ,//62
{114,125,115,114,0,0,0,0,0,0,0,0,0,0,0} ,//63
{114,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//64
{115,118,116,0,0,0,0,0,0,0,0,0,0,0,0} ,//65
{115,117,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//66
{116,126,118,0,0,0,0,0,0,0,0,0,0,0,0} ,//67
{116,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//68
{117,19,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//69
{117,20,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//70
{118,120,119,0,0,0,0,0,0,0,0,0,0,0,0} ,//71
{119,123,120,119,0,0,0,0,0,0,0,0,0,0,0} ,//72
{119,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//73
{120,122,121,0,0,0,0,0,0,0,0,0,0,0,0} ,//74
{121,124,122,121,0,0,0,0,0,0,0,0,0,0,0} ,//75
{121,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//76
{122,52,113,53,0,0,0,0,0,0,0,0,0,0,0} ,//77
{122,98,0,0,0,0,0,0,0,0,0,0,0,0,0},//78   /////////////------------------Stupid me forgot this rule
///////////------------------------------ADD 1 to the following grammar rule nos
{123,33,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//78
{123,34,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//79
{124,35,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//80
{124,36,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//81
{125,24,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//82
{125,25,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//83
{126,37,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//84
{126,38,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//85
{126,40,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//86
{126,39,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//87
{126,41,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//88
{126,42,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//89
{127,10,108,45,91,47,0,0,0,0,0,0,0,0,0} ,//90
{128,28,52,54,53,8,129,132,9,0,0,0,0,0,0} ,//91
{129,29,131,45,95,30,47,130,0,0,0,0,0,0,0} ,//92
{130,29,131,45,95,30,47,130,0,0,0,0,0,0,0} ,//93
{130,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//94
{131,57,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//95
{131,19,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//96
{131,20,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//97
{132,31,45,95,30,47,0,0,0,0,0,0,0,0,0} ,//98
{132,1,0,0,0,0,0,0,0,0,0,0,0,0,0} ,//99
{133,26,52,54,27,134,53,8,95,9,0,0,0,0,0} ,//100
{133,32,52,98,126,98,53,8,95,9,0,0,0,0,0} ,//101
{134,57,46,57,0,0,0,0,0,0,0,0,0,0,0} ,//102
};	
struct token_store_node
{
	char lexeme[30];
	int line_number;
	char token[30];
	struct  token_store_node* next;
	
};
typedef struct token_store_node token_store_node;
token_store_node * head;
token_store_node * currrent; 

struct FIRST_STRUC
{
	int sym;
	int rule;
}first[NT_COUNT][MAX_FIRST];
int follow[NT_COUNT][MAX_FOLLOW];
int parseTable[NT_COUNT][T_COUNT-1];

enum TERMINALS {EPSILON=1,END_OF_STRING,INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, GET_VALUE, PRINT, USE, WITH,
				PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN , SWITCH, CASE, BREAK, DEFAULT, WHILE,
				PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP,
				SQBO, SQBC, BO, BC, ID, DRIVERDEF, DRIVERENDDEF} terminal;
				
enum NON_TERMINALS	{program=80,moduleDeclarations,moduleDeclaration,otherModules,driverModule,module,ret,input_plist,more_input,output_plist,more_output,
					 dataType,type,moduleDef,statements,statement,ioStmt,print_var,var,whichId,simpleStmt,assignmentStmt,whichStmt,lvalueIDStmt,lvalueARRStmt,
					 index,moduleReuseStmt,optional,idList,more_id,expression,U,more_u,arithOrBoolExpr,more_abexp,AnyTerm,more_arith,boolConst,arithmeticExpr,more_term,
					 term,more_factor,factor,pl_mi,mu_di,logicalOp,relationalOp,declareStmt,conditionalStmt,caseStmt,more_case,value,defaultStmt,iterativeStmt,
					 range}	non_terminal;