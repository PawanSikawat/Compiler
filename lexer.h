/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexerDef.h"

int hashvaluer(char *word);
void getStream(FILE *fp);
void removeComments(FILE *ip,FILE *op);
void reportLexError(int count, char *err);
int getNextToken(FILE *fp);