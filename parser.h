/* Group No - 38
Samanvay Lamba - 2017A7PS0022P
Pawankumar Sikawat  - 2017A7PS0022P
Harsh Khatri - 2017A7PS0055P
Siddhant Dangi - 2017A7PS0074P */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserDef.h"

struct Stack* createStack(unsigned capacity);
bool isFull(struct Stack* stack);
bool isEmpty(struct Stack* stack);
int push(struct Stack* stack, int item);
int pop(struct Stack* stack);
int peek(struct Stack* stack);
bool checkPresent(int curr, int find);
void computeFirstAndFollowSets();
void printFirst();
void printFollow();
void printTokenStream(int tk);
void createParseTable();
void reportSynError(int tk);
void parseInputSourceCode(FILE *fp, FILE *op);