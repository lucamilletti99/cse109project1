/* 
 * File: prog1_funcs.c
 * Name: Luca Milletti
 * UID: lpm222
 */
#include <stdio.h> //include fprintf, fget, fclose etc..
#include <stdlib.h> 
#include <string.h> //include strings
#include <errno.h> //includes errno 
#include <stdbool.h>
#include "prog1_funcs.h"

void parseArgs(int argc, char *argv[], FILE **IFile){
  switch(argc){ //unless the command line  is two arguments, the help message is printed
    case(2):
      *IFile = Fopen(argv[1],"r");
      break;
    default:
      help();
      break;
  }
}
void parseFile(FILE *IFile, Number **headNum){ //looks through file and parses contents to look for floats and invalid lines
  size_t buffSize = 0; //allocates correct amount of memory depending on situation if buffSize is initialized to 0
  float numFloat; //holds value of float
  char *lineHolder; //holds the line, similar to a buffer, from getline(): getline automatically allocated memory for lineHolder buffer 
  unsigned int lineCount = 1; //holds the value of lines 
  ssize_t lineSize = getline(&lineHolder,&buffSize,IFile); //sets string to lineHolder, linesize is number of 
  bool floatFound = false; //set to positive if a float is found, otherwise no float is found 
  while(lineSize != EOF && (lineHolder)[lineSize -1] == '\n'){ //getline returns  -1 at EOF terminator
    (lineHolder)[lineSize -1] = '\0'; //reduces given that getline returns a positive value to lineSize(which we check for in the while loop)
    if(*lineHolder == '\0') fprintf(stderr,"Line %d - empty\n",lineCount);
    else if(sscanf(lineHolder,"%f", &numFloat) == 1){ //assuming that sscanf correctly reads a float, it will input the value into numFloat
      floatFound = true;
      recordNum(headNum,numFloat); //recordNum called to enter numFloat into the linked list(headNum)
    }
    else fprintf(stderr,"Line %d - no float found\n",lineCount);
    lineCount++;
    lineSize = getline(&lineHolder,&buffSize,IFile); //calls getline again to read next line
  }
  if(lineHolder) free(lineHolder);
  Fclose(IFile);
  if(!floatFound) bail(20,"No values in file");
}
void *Malloc(size_t numBytes){//allocates memory and returns the void pointer to the address of the allocated memory                                               
  void *ptr = NULL;                                                      
  if ((ptr = malloc(numBytes)) == NULL){//Steven's Style wrapper function                            
    bail(99, "Could not allocate space");
    exit(99);                                                            
  }
  return ptr;
}
FILE *Fopen(const char *path, const char *mode){ 
  FILE *fp = NULL;
  char str[128];
  if((fp=fopen(path, mode)) != NULL) return fp; //error checks that fp is not Null
  else{ //if it is null, this file open error is printed to stderr
    sprintf(str,"Unable to open %s with mode %s",path,mode);
    bail(10, str);
    exit(10);
  }
}
void Fclose(FILE *fp){ 
  if(fclose(fp)!= 0){ //error checks to see if fclocse(fp) correctly closes File *fp
    bail(11, "Unable to open close file");
    exit(11); 
  }
}
void bail(int rc, const char *msg){  //bails with code RC and message 'msg'
  //made sure to print the error msg to stderr and if specified, print message with errno
  if(errno) fprintf(stderr, "(%d) %s - %s\n",rc,msg,strerror(errno));
  else fprintf(stderr, "(%d) %s\n",rc,msg);
  exit(rc);
}
void help(){
  fprintf(stdout,"\nprog1 reads a file of floating-point numbers and stores them in a linked list for optional sorting.\n");
  fprintf(stdout,"\nAfter reading, the user is queried to sort or print the numbers in the order entered (i.e., as is).");
  fprintf(stdout,"\nThe user can sort in increasing or decreasing order, and to sort by the value or its count.");
  fprintf(stdout,"\nRegardless of the choice(s) made, the values and their counts are printed.");
  fprintf(stdout,"\n\nNote: Any duplicates in the file will be counted but not stored as a duplicate in the list.");
  fprintf(stdout,"\n\tIf a line does not contain a number, that line number and its contents are printed to stderr.");
  fprintf(stdout, "Return codes are as follows. Any error message is printed to stderr.\n");
  fprintf(stdout, " 0 - normal execution (even if it results in help message)\n");
  fprintf(stdout, "10 - Unable to open the file specified\n");
  fprintf(stdout, "11 - Unable to close the file specified\n");
  fprintf(stdout, "20 - No value were found in the file\n");
  fprintf(stdout, "99 - Unable to allocate space\n");
  exit(0);
}
void queryModeAndKey(char *mode, char *key){
  char *inputMode = Malloc(sizeof(char *)); //allocate memory temporarily for a user input mode
  char *inputKey = Malloc(sizeof(char *)); //allocate memory temporarily for a user input key
  bool validMode = false; //to make sure first while loop runs correctly
  bool validKey = false; //to make sure second while loop runs correctly
  *key = 0; *mode = 0; //resets mode and key to not have a value;
  while(!validMode){
    fprintf(stdout, "Enter sort mode - A for \"As entered\", I for \"Increasing\", D for \"Decreasing\": ");
    if(scanf("%s",inputMode) == 1){ //checks if value passed in is only one byte (aka a char)
      if(strcmp(inputMode, "I") == 0 || strcmp(inputMode, "D") == 0){ //(case sensitive) check on the inputMode
        getchar(); //gets rid of extra input read from scanf
        validMode = true;
        while(!validKey){ //repeats if a valid key is not entered, boolean unnecessary because of exit call 
          fprintf(stdout, "Enter sort key - N for \"Number\", C for \"Count\": ");
          if(scanf("%s",inputKey) == 1 && strcmp(inputMode, "A") != 0){ //checks for character again
            if(strcmp(inputKey, "C") == 0 || strcmp(inputKey, "N") == 0){
              *key = *inputKey; //makes sure to set value back in main to current key
              *mode = *inputMode; //same as above
              validKey = true;
            }
            else{
              getchar(); //gets rid of invalid input
            }
          }    
        }
      }
      else if(strcmp(inputMode, "A") == 0){
        *mode = *inputMode; 
        validMode = true;
      }
    }
  }
  if (inputMode) free (inputMode); //free memory allocated for variable
  if (inputKey) free(inputKey);
  return;
}
void printNums(Number *headNum, char mode, char key){ //iterates through linked list and prints values, simultaneously freeing the memory after printing the vlaue
  if(mode == 'I'){
    headNum = MergeSort(headNum, key);
  }
  else if(mode == 'D'){
    headNum = MergeSort(headNum,key);
    headNum = reverseList(headNum);
  }
  while (headNum != NULL){
    /*print the linked list out*/
    fprintf(stdout, "%+2.3E  -  %d\n", headNum->value, headNum->count);
    Number *prevNum = headNum; //prevNum is meant to act similar to currNum, and hold the previous value of the linked list which is then freed at the end of the loop
    headNum = headNum->next; //moves to next node in linked list
    free(prevNum);
  }
  return;
}
void recordNum(Number **headNum, float value){
  if(*headNum == NULL){ //the list is empty
    *headNum = (Number *)Malloc(sizeof(Number));  //allocates space for a new number to be put into the linked list
    (*headNum)->count = 1; //sets count to 1
    (*headNum)->value = value;
    (*headNum)->next = NULL; //always set NULL at end of linked list
  }
  else{ //the list has values
    Number *currNum = *headNum; //creates a pointer to store the values of the list, acts like an iterator through the linked list
    while(currNum != NULL){ //if finds value it will increment the count and if not adds to a new node
      if(currNum->value == value){ //checks if value already exists
        currNum->count = currNum->count + 1; //increments count
        break;
      }
      else if(currNum->next == NULL){ //end or 'tail' node set to null
        Number *newNum  = (Number *) Malloc(sizeof(Number)); //new number is to be added
        newNum->count = 1; //sets count of newNumber to 1
        newNum->value = value; //sets newNum to equal input float
        newNum->next = NULL;
        currNum->next = newNum; //add new number to end of list or last node
        break;
      }
      else{//shift to next node in list
        currNum = currNum->next;
      }
    }
  }
}
Number *MergeSort(Number *headNum, char key) { 
    Number *front; 
    Number *back; 
  
    if ((headNum == NULL) || (headNum->next == NULL)) { //if linked list only has 1 or less nodes initialized
        return headNum;
    } 
    else{
      splitList(headNum, &front, &back); //splits linked list into two halved linked list, recursively called by mergesort 
      front = MergeSort(front,key); //sort front recursively
      back = MergeSort(back,key);  //sort back recursively
      return sortedMerge(front, back, key); //merge sorted lists together
    }
    
}
/*Ideas for splitLsit and sortedMore taken from https://www.youtube.com/watch?v=jOyYaBHyN28 , Quinston Pimenta*/
void splitList(Number *headNum, Number** frontPtr, Number** backPtr) { 
    Number *fastPtr;
    Number *slowPtr;
    /*Purpose of this segment is: we set fastPtr equla to headnum and fast pointer to the next value after that
      Everytime the while loop runs, the goal is to increment fast pointer twice as fast as slow pointer so that by the time fastPtr 
      reaches the end of the linked list, we can use the slow pointer to mark the midpoint of the linked list to set to the backRef pointer*/
    slowPtr = headNum;
    fastPtr = headNum->next;
    while(fastPtr != NULL){
      fastPtr = fastPtr->next;
      if(fastPtr != NULL){
         fastPtr = fastPtr->next;
         slowPtr = slowPtr->next;
      }
    }
    *frontPtr = headNum; //sets beginning of list to headNum
    *backPtr = slowPtr->next; //sets back reference pointer to midpoint of linked list
    slowPtr->next = NULL; //splits linked list in half at midpoint
}
Number *sortedMerge(Number* front, Number* back, char key){ 
    Number *newList = NULL; //ready to send back the pointer to the new head of the linked lsit
    /* Base cases */
    if (front == NULL) 
        return (back); 
    else if (back == NULL) 
        return (front); 
    if(key == 'N'){ //sort by value
      if (front->value <= back->value) { 
          newList = front; 
          newList->next = sortedMerge(front->next, back,key); 
      } 
      else { 
          newList = back; 
          newList->next = sortedMerge(front, back->next, key); 
      } 
    }
    else{ //sort by count
      if (front->count < back->count) { 
          newList = front; 
          newList->next = sortedMerge(front->next, back, key); 
      } 
      else { 
          newList = back; 
          newList->next = sortedMerge(front, back->next, key); 
      } 
    }
    return newList; 
} 
Number *reverseList(Number *headNum){
  //we create three pointers to act as temporary holders why we swap the direction of the pointers
  Number *prevNode = NULL; //left most node when going through linked list
  Number *currNode = headNum; //middle node
  Number *nextNode = headNum; //right node 
  if(headNum == NULL || headNum->next == NULL){
    return headNum;
  }
  else{
    /*Rather than trying to swap all of the elements of the linked list, we can actually just reverse the direction that the 
    the pointers are "facing". Instead of a node pointing to the next node, we can swap it look at the previous node so that the linked list
    and the way it iterates, it looks at the linked list as though it was reverse*/
    while(currNode != NULL){
      nextNode = nextNode->next; //move the next node to 
      currNode->next = prevNode;
      prevNode = currNode;
      currNode = nextNode;
    }
  }
  return prevNode; //return the previous value, which is actually the last "iterator"
}
void printList(Number *headNum){//used to print individual lists
  Number *currNum = headNum;
  while(currNum){
    printf("%f ->",currNum->value);
    currNum = currNum->next;
  }
  printf(" \n");
}