#include "assembler.h"

int label_is_legal(char *str)
{
  int i=0; 
  if(strlen(str) > MAX_LABEL_LEN) /*label can't be bigger than 31 characters*/
    return print_error("illegal label");
  if(!(isalpha(str[i]))) /*label has to start with an alphabetic letter*/
    return print_error("illegal label");
  for(i=1; i < strlen(str)-1; i++) 
  {
    if(!(isalpha(str[i])) && !(isdigit(str[i]))) /*the others letters have to be an alphabetic letter or a number*/
      return print_error("illegal label");
  }
  if(is_op(str) || is_directive(str) || is_reg(str)) /*the label name can't be same as operation/directive/regiester name*/
    return print_error("illegal label");
  return TRUE; /*return true if there were no errors*/
}

int multiple_label(char* str)
{
    symbol_ptr temp = head;
    for (; temp != NULL; temp = temp->next) /*checks if this label already entered to the synbol table*/ 
    {
	if(!strcmp(temp->type, "external") || !strcmp(temp->type, "entry"))
	    continue; /*this is ok to declare on more than one extern/entry with the same name*/ 
        if (strcmp(temp->symbol, str) == STR_EQUALS)
            return print_error("multipale label");
    }
    return TRUE; /*return true if there were no errors*/
}




int is_label(char *str)
{ 
  return str[strlen(str) - 1] == ':'; /*by definition, a word that ends with a colon is a label*/
}

int is_op(char *str)
{
  int row;
  for(row = 0; row < NUM_OF_OPS; row++)
  {
    if(strcmp(instructions[row][0],str) == STR_EQUALS) /*checks if this string is one of the 16 operations*/
      return TRUE; 
  } 
  return FALSE;
}

int is_directive(char *str)
{
  /*checks if this string is a directive*/
  if(!(strcmp(".data",str)) || !(strcmp(".string",str)) || !(strcmp(".entry",str)) || !(strcmp(".extern",str)))
    return TRUE; 
  return FALSE;
}

int is_reg(char *str)
{
  /*checks if this string is one of the seven registers*/
  if(strlen(str) == 2 && str[0] == 'r' && str[1] >= '0' && str[1] < '8')
    return TRUE;
  return FALSE;
}

void add_symbol(char* name, int address, char* type)
{
    symbol_ptr curr = head;
    symbol_ptr add = calloc(1, sizeof(struct symbol_table)); /*a node to add to the linked list (in the last place)*/
    if (add == NULL)
    {
        printf("memory allocation failed");
        exit(0); /*exit the program if the memory allocation failed*/
    }
    strcpy(add->symbol, name); /*set the symbol (to the name of the label)*/
    add->address = address; /*set the address*/
    strcpy(add->type, type); /*set the type*/
    add->next = NULL; /*set the next to be null*/
    if (!head) /*if the list is empty, we shuold add the node to the beginning of the list*/
    {
        head = add;
    }
    else /*otherwise, we shuold add the node to the end of the list*/
    {
        for (; curr->next != NULL; curr = curr->next);
        curr->next = add;
        curr->next->next = NULL;
    }
}

void free_symbol_table(symbol_ptr head)
{
	if(head == NULL) /*the stop condition*/
		return;
	free_symbol_table(head->next); /*the recursivian call*/
	free(head); /*free the symbol table (from the last to the first)*/
}
 
      


    	
