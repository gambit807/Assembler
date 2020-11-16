#include "assembler.h"

int IC, DC, ICF, DCF, line_number, ex_index;
char file_name[MAX_ARRAY_LEN];
symbol_ptr head;  /*the first label in the symbol table*/
word code_array[MAX_ARRAY_LEN]; /*the instruction image*/
long data_array[MAX_ARRAY_LEN]; /*the data image*/
ext_struct arrex[MAX_ARRAY_LEN]; /*the array contains the appearances of the extern labels*/


char* instructions[16][4] = {
  {"mov","0","0","2"},
  {"cmp","1","0","2"},
  {"add","2","1","2"},
  {"sub","2","2","2"},
  {"lea","4","0","2"},
  {"clr","5","1","1"},
  {"not","5","2","1"},
  {"inc","5","3","1"},
  {"dec","5","4","1"},
  {"jmp","9","1","1"},
  {"bne","9","2","1"},
  {"jsr","9","3","1"},
  {"red","12","0","1"},
  {"prn","13","0","1"},
  {"rts","14","0","0"},
  {"stop","15","0","0"}}; /*2D array of the instructions and their properties*/

int operands_number; /*the variable should contain the operands number of the instruction that is parsed*/

int pass_one(FILE *input)
{
  char line[MAX_LINE_LEN];
  int errors = 0; /*counts the errors in every line parsing*/
  symbol_ptr temp_ptr;
  head = (symbol_ptr)malloc(sizeof(struct symbol_table));
  if (!head)
  {
      printf("memory allocation failed"); /*dynamic memory failure*/
      return FALSE;
  }
  head = NULL;
  IC = IC_START;
  DC = DC_START;
  if(input)
  {
    temp_ptr = head;
    for(line_number = 1; fgets(line, MAX_LINE_LEN, input); line_number++)
    {
      if(line_parsing(line) == FALSE) /*if the line had an error*/
        errors++;
    }
    if(errors)/*if the first pass found errors, the file should not be sended to the second pass*/
    {
      printf("there were %d errors\n", errors);   
      return FALSE; 
    }
    ICF = IC; /*saving the index of the last word in the code array*/
    for(temp_ptr = head; temp_ptr != NULL; temp_ptr = temp_ptr->next)
    {
      if(strcmp(temp_ptr->type,"data") == STR_EQUALS)
        temp_ptr->address += (IC); /*the data words should come after the code words, and so the labels*/
    }
    rewind(input); /*going back to the start of the file for the second pass*/
    return pass_two(input);
  }
  return FALSE; /*the file contains null*/
}

  
