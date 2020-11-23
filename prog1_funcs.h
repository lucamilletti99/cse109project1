/* 
 * Program: prog1_funcs.h
 * Name: Mark Erle
 * Userid: merle
 */

#ifndef PROG1_FUNCS
#define PROG1_FUNCS

#include <stdio.h>    /* FILE */

struct number {
  float value;
  unsigned int count;
  struct number *next;
};
typedef struct number Number;

void parseArgs(int, char *[], FILE **);   /* in: argc, argv, pointer to pointer to file name (need pointer to pointer so we can change in main) */
void parseFile(FILE *, Number **);        /* in: pointer to input file, pointer to pointer to head/first number */ 
void recordNum(Number **, float);         /* in: pointer to pointer to head/first number, number to store or increment count */ 
void queryModeAndKey(char *, char *);     /* pointer to char to indicate mode (need pointer so we can change in main) */
void printNums(Number *, char, char);     /* in: pointer to head/first number, char to indicate mode */
void *Malloc(size_t);                     /* in: bytes to allocate, return: pointer (type void) to allocated memory */
FILE *Fopen(const char *, const char *);  /* in: path/file, mode; return: pointer to type FILE */
void Fclose(FILE *);                      /* in: pointer to type FILE */
void bail(int, const char *);             /* in: return code, message; exit with rc after printing message to stderr */
void help();                              /* print help message to stdout and exit with rc = 0 */
Number *MergeSort(Number *, char);
void splitList(Number *,Number **, Number **);
Number *sortedMerge(Number *, Number *, char);
int Length(Number *);
Number *reverseList(Number *);
void printList(Number *);
#endif
