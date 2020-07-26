/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#ifndef codegenh
#define codegenh
//#include "astDef.h"

int marker;

void generate_asmcode_util(char* str);
void generate_asmcode();
void generate_data(bptr b_root);
void generate_asm(astptr a_root,bptr b_root);

FILE* asm_file;


#endif