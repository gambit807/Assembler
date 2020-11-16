#include "assembler.h"

int parse_two(char *line)
{
	char* token; /*gets every part in the line by the function "strtok"*/
	symbol_ptr temp;
	token = strtok(line, " \t\n"); /*getting the first word*/
	if (!token || *token == ';') /*if it's an empty line or a note line*/
	{
		return TRUE;
	}
	if(is_label(token)) /*if the first word is a label*/
	{
		token = strtok(NULL, " \t\n"); /*getting the next word*/
	}
	if(!strcmp(".data",token) || !strcmp(".string",token) || !strcmp(".extern", token))
	{
		return TRUE; /*we already checked this cases in pass one*/
	}
	if(!strcmp(".entry",token)) /*checks if the word is an entry directive*/
	{
		token = strtok(NULL, " \t\n"); /*getting the label after the entry directive*/
		for(temp = head; temp != NULL; temp = temp->next)
		{
			if(!strcmp(temp->symbol,token)) /*checks if this label appears in the symbol table*/ 
			{
				strcpy(temp->type, "entry"); /*set the type of this label to be "entry"*/
				break;
			}
		}
		if(!temp) /*prints error if this label wasn't appear in the symbol table*/ 
			return print_error("undefined label");
		return TRUE;
	}
	operands_number = operands(token); /*if the word isn't a directive, it's an operation*/
	if(operands_number == 0) 
	{
		IC++; /*this line occupies space of one word of memory*/ 
		return TRUE;
	}
	else if(operands_number == 1)
	{
		token = strtok(NULL, " \t\n"); /*getting the first operand*/
		if(*token == '&') /*the operand is a label with relative addressing*/
		{
			IC++; /*the next word of memory is for the jump distance*/ 
			token++; /*takes just the label (skipping over the '&' character)*/
			for(temp = head; temp != NULL; temp = temp->next) /*checks if this label appears in the symbol table*/
			{
				if(!strcmp(temp->symbol,token)) /*this label appears in the symbol table*/
				{ 
					value_to_word((temp->address) - IC + 1); /*set the word of memory to the jump distance*/
					if(temp->address == 0) /*if it is an extern label*/
					{
						strcpy(arrex[ex_index].label, temp->symbol);
						arrex[ex_index].address = IC;
						ex_index++;
						code_array[IC-100].E = 1; /*this is external*/
						IC++; /*this line occupies space of two words of memory*/ 
						return TRUE; 
					}
					else
					{
						IC++; /*this line occupies space of two words of memory*/ 
						return TRUE; 
					}
				}
			}
			if(!temp) /*if this label didn't appear in the symbol table*/
				return print_error("undefined label");
		}
		else if(*token != '#' && !is_reg(token)) /*if it is a label*/
		{
			IC++; /*the next word of memory is for the address of the label*/ 
			for(temp = head; temp != NULL; temp = temp->next) /*checks if this label appears in the symbol table*/
			{
				if(!strcmp(temp->symbol,token)) /*this label appears in the symbol table*/
				{ 
					value_to_word(temp->address); /*set the word of memory to the address of the label*/
					if(temp->address == 0) /*if it is an extern label*/
					{
						strcpy(arrex[ex_index].label, temp->symbol);
						arrex[ex_index].address = IC;
						ex_index++;
						code_array[IC-100].E = 1; /*this is external*/
						IC++; /*this line occupies space of two words of memory*/ 
						return TRUE; 
					}
					else
					{
						code_array[IC-100].R = 1; /*this is relocatable*/
						IC++; /*this line occupies space of two words of memory*/ 
						return TRUE; 
					}
				}
			}
			if(!temp) /*if this label didn't appear in the symbol table*/
				return print_error("undefined label");
		}
		else
		{
			if (*token == '#') /*if the operand is a number*/
				IC += 2; /*this line occupies space of two words of memory*/ 
			else /*the operand is a register*/
				IC++; /*this line occupies space of one word of memory*/ 
		}
	}
	else /*there is two operands*/
	{
		token = strtok(NULL, " ,\t\n"); /*getting the first operand*/
		if(*token != '#' && !is_reg(token)) /*if it is a label*/
		{
			IC++; /*the next word of memory is for the address of the label*/ 
			for(temp = head; temp != NULL; temp = temp->next) /*checks if this label appears in the symbol table*/
			{
				if(!strcmp(temp->symbol,token))
				{ /*this label appears in the symbol table*/
					value_to_word(temp->address); /*set the word of memory to the address of the label*/
					if(temp->address == 0) /*if it is an extern label*/
					{
						strcpy(arrex[ex_index].label, temp->symbol);
						arrex[ex_index].address = IC;
						ex_index++;
						code_array[IC-100].E = 1; /*this is external*/
						IC++; /*this line occupies space of at least two words of memory*/ 
					}
					else
					{
						code_array[IC-100].R = 1; /*this is relocatable*/
						IC++; /*this line occupies space of at least two words of memory*/
					}
					break;
				}
			}
			if(!temp) /*if this label didn't appear in the symbol table*/
				return print_error("undefined label");
		}
		else 
		{
			if (*token == '#') /*if the first operand is a number*/
				IC+=2; /*this line occupies space of at least two words of memory*/
			else
				IC++; /*this line occupies space of at least one word of memory*/
		}
		token = strtok(NULL," ,\t\n"); /*getting the second operand*/
		if(*token != '#' && !is_reg(token)) /*if it is a label*/
		{
			for(temp = head; temp != NULL; temp = temp->next) /*checks if this label appears in the symbol table*/
			{
				if(!strcmp(temp->symbol,token))
				{ /*this label appears in the symbol table*/
					value_to_word(temp->address); /*set the word of memory to the address of the label*/
					if(temp->address == 0) /*if it is an extern label*/
					{
						strcpy(arrex[ex_index].label, temp->symbol);
						arrex[ex_index].address = IC;
						ex_index++;
						code_array[IC-100].E = 1; /*this is external*/
						IC++; /*this line occupies space of at least two words of memory*/
						return TRUE; 
					}
					else
					{
						code_array[IC-100].R = 1; /*this is relocatable*/
						IC++; /*this line occupies space of at least two words of memory*/
						return TRUE; 
					}
				}
			}
			if(!temp) /*if this label didn't appear in the symbol table*/
				return print_error("undefined label");
		}
		else if (*token == '#') /*if the second operand is a number*/
			IC++; /*this line occupies space of at least two words of memory*/
	}
	return TRUE;
}

			
	

int pass_two(FILE *input)
{
	char line[MAX_LINE_LEN];
	int errors = 0; /*counts the errors in every line parsing*/
	ex_index = 0;
	IC = IC_START; /*set IC to be 100 again*/
	for(line_number = 1; fgets(line, MAX_LINE_LEN, input); line_number++)
	{
		if(parse_two(line) == FALSE) /*if the line had an error*/
        	errors++;
	}
	if(errors)
	{
    		printf("there were %d errors\n", errors);
    		return FALSE; /*if there were errors, this function should return false*/
	}
	return TRUE; /*there were no errors during the second pass*/
}



	
    
