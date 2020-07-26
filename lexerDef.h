/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOKENCOUNT  30

struct token
{
    char name[15];
    int value;
};
struct token hashtable[TOKENCOUNT] = {{ "AND", 24} ,//1
{ "array", 7} ,//2
{ "boolean", 5} ,//6
{ "break", 30} ,//7
{ "case", 29} ,//8
{ "declare", 10} ,//11
{ "default", 31} ,//13
{ "driver", 12} ,//15
{ "end", 9} ,//18
{ "false", 20} ,//21
{ "for", 26} ,//22
{ "get_value", 14} ,//24
{ "in", 27} ,//26
{ "input", 22} ,//27
{ "integer", 3} ,//28
{ "module", 11} ,//32
{ "of", 6} ,//36
{ "OR", 25} ,//37
{ "parameters", 18} ,//38
{ "print", 15} ,//40
{ "program", 13} ,//41
{ "real", 4} ,//43
{ "returns", 23} ,//44
{ "start", 8} ,//50
{ "switch", 28} ,//51
{ "takes", 21} ,//52
{ "true", 19} ,//53
{ "use", 16} ,//54
{ "while", 32} ,//55
{ "with", 17} ,//56
};

const int buff_size = 250;

int fileRead_count = 0;
char lexeme[20];
int lex_len = 0;
char buffer[buff_size];
int buff_pos = 0;
bool end_file = false;
int line_count = 1;

bool dont = true;
bool dd = false;
char ch;