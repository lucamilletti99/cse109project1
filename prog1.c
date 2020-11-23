/* 
 * Program: prog1.c
 * Name: Mark Erle
 * Userid: merle
 */

/* preprocessor directives */
#include "prog1_funcs.h"

/* main */
int main(int argc, char *argv[]) {
  FILE *Ifile = NULL;
  Number *headNum = NULL;
  char mode = 'A';         /* 'A' for "As entered" */
  char key = 'N';          /* 'N' for "Number" */
  parseArgs(argc, argv, &Ifile); 
  parseFile(Ifile, &headNum);
  queryModeAndKey(&mode, &key);
  printNums(headNum, mode, key);
  return 0;
}
