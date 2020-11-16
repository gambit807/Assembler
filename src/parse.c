#include "assembler.h"

int line_parsing(char *line)
{
  symbol_ptr temp = head; /*contains at first the first element in the symbol tabel*/
  int label_flag = FALSE; /*contains true if there was a label in the beginning of the line*/
  char *token; /*gets every part in the line by the function "strtok"*/
  char label[MAX_LABEL_LEN + 1]; /*contains the label's name for entering it to the symbol table*/
  char copy[MAX_LINE_LEN+1]; /*contains a copy of the line for avoiding errors later*/
  strcpy(copy, line);
  token = strtok(line," \t\n"); /*getting the first word*/
  if(!token || *token == ';') /*if it's an empty line or a note line*/
  {
    return TRUE;
  }
  if(is_label(token)) /*if the first word contains a colon in the end*/
  {
    token = strtok(copy, " :\t\n"); /*removing the colon from the label*/
    if (!label_is_legal(token) || !multiple_label(token)) /*if the label is not a legal label that wasn't defined before*/
        return FALSE;
    label_flag = TRUE; 
    strcpy(label, token);
    token = strtok(NULL, " \t\n");
    if (!token) /*if there is nothing after the label*/
    {
        return print_error("there is nothing after the label");
    }
  }
  if(!strcmp(".data",token) || !strcmp(".string",token)) /*if the line is a data or string line*/
  {
      if(label_flag)
      {
        add_symbol(label,DC,"data"); /*adding the symbol to the symbol table as a data*/
      }
      if(strcmp(".data",token) == STR_EQUALS) /*if the line is a data directive line*/
      {
        token = strtok(NULL, "\n");
        return get_data(token);
      }
      if(strcmp(".string",token) == STR_EQUALS) /*if the line is a string directive line*/
      {
        token = strtok(NULL, "\n");
        if(!get_string(token))
          return FALSE;
        return TRUE;
      }
  }
  else if(is_directive(token)) /*if the line has an entry or extern directive*/
  {
    if(strcmp(".entry",token) == STR_EQUALS) /*if the line has an entry directive*/
    {
        token = strtok(NULL, " \t\n");
        if (!token) /*if there is nothing after the ".entry"*/
            return print_error("there is no label after the entry directive");
        token = strtok(NULL, " \t\n");
        if (!token)
            return TRUE;
        return print_error("illegal entry directive"); 
    }
    if(strcmp(".extern",token) == STR_EQUALS) /*if the line has an extern directive*/
    {
      token = strtok(NULL," \t\n");
      if (!token) /*if there is nothing after the ".entry"*/
          return print_error("there is no label after the extern directive");
      if(label_is_legal(token) && multiple_label(token))  /*if the label is a legal label that wasn't defined before*/
      {       
         strcpy(label,token);
         add_symbol(label, 0, "external"); /*if adding the symbol to the symbol table as an external*/
         token = strtok(NULL," \t\n");
         if (token) /*if there is another word after the label*/
           return print_error("illegal extern directive");
         return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
  }  
  if(label_flag)
  {
    add_symbol(label, IC, "code"); /*adding the symbol to the symbol table as a code*/
  }
  if (!opcodes(token)) /*if the instruction is not one of the 16 defined instructions*/
    return FALSE;
  if(operands_number == 0)
  {
    code_array[IC-100].A = 1; 
    IC++;
    token = strtok(NULL," \t\n");
    if(token)
	print_error("too many operands");
    return TRUE; 
    
  }
  token = strtok(NULL,"\n");
  if (!instruction_line(token))  /*if the parse of the operands found an error*/
    return FALSE;
  return TRUE;
}
    
      
          

