#include "assembler.h"

int instruction_line (char *str)
{
	int curr,curr_IC, comma = FALSE; /*"comma" is a flag (there should be no more than one comma)*/
	char *token, *temp = str;
	char ch = 0;
	if(str == NULL) /*if the string is empty and therefore there aren't any operands*/
	{
		return print_error("there aren't operands in instruction line");
	} 
	if(operands_number == 1)
	{
		token = strtok(str, " \t\n");
		if(!token) /*if the string is empty and therefore there aren't any operands*/
			return print_error("too little operands");
		if(*token == '#') /*if the operand is a number*/
		{
			token++; 
			if(code_array[IC-100].opcode != 13) /*only prn can get a number as an operand*/
			{
				return print_error("illegal operand");
			}
			if(get_number(token) == TOO_BIG_NUMBER) /*if the number is illegal*/
			{
				return print_error("illegal number in the operand");
			}
			curr = get_number(token);
			code_array[IC-100].A = 1; 
			IC++; 
			if(!value_to_word(curr)) /*converting the number into a word and checking if the converting was successful*/
				return FALSE;
			code_array[IC-100].A = 1; 
			IC++; 
		}
		else if(is_reg(token)) /*if the operand is a register*/
		{
			if(code_array[IC-100].opcode == 9) /*only jmp,bne,jsr can't get a register as an operand*/
			{
				return print_error("illegal operand");
			}	
			code_array[IC-100].dest_addressing = 3;
			code_array[IC-100].dest_reg = token[1]-48; /*-48 is the differance between a digit character to its ascii code*/ 
			code_array[IC-100].A = 1;
			IC++;
		}
		else if(*token == '&') /*if the operand is a label with relative addressing*/
		{
			token++;
			if (!label_is_legal(token)) /*if the label after the "&" is illegal*/
			{
				return FALSE;
			}
			if(code_array[IC-100].opcode != 9) /*only jmp,bne,jsr can get a '&' as an operand*/
			{
				return print_error("illegal operand");
			}
			code_array[IC-100].dest_addressing = 2; 
			code_array[IC-100].A = 1;
			IC++;
			code_array[IC-100].A = 1;
			IC++;
		}
		else /*if the operand is a label*/
		{
			if (!label_is_legal(token)) /*if the label is illegal*/
			{
				return FALSE;
			}
			code_array[IC-100].dest_addressing = 1;
			code_array[IC-100].A = 1;
			IC += 2;
		} 
		token = strtok(NULL, " \t\n");
		if(token) /*if there is another word after the operand*/
		{
			return print_error("too many operands");
		}
		return TRUE;
	}
	else
	{
		curr_IC = IC; 
		while(*temp) /*counting the commas*/
		{
			if(*temp == ',')
				comma++;
			temp++;
		}
		if(!comma) /*if there wasn't any comma*/
			return print_error("there is no comma between the operands");
		if(comma != TRUE) /*if there were more than one comma*/
			return print_error("too many commas");
		temp = str;
		while (is_white(*temp))
		{
			temp++;
		} /*getting the first char that is not white*/
		if(*temp == ',') /*if the first char that is not white is a comma*/
			return print_error("first operand can't start with a comma");
		for(; *temp != '\0'; temp++)
		{
			if(!is_white(*temp))
				ch = *temp; /*ch gets the last char that is not white*/
		}
		if(ch == ',') /*if the last char that is not white is a comma*/
			return print_error("the operands can't end with a comma");  
		token = strtok(str," ,\t");
		if(*token == '#')  /*if the operand is a number*/
		{
			token++; 
			if(code_array[IC-100].opcode == 4) /*only lea can't get a number as an operand*/
			{
				return print_error("illegal operand");
			}
			if(get_number(token) == TOO_BIG_NUMBER) /*if the number is illegal*/
			{
				return print_error("illegal number in the operand");
			}
			curr = get_number(token);
			code_array[IC-100].A = 1; 
			IC++; 
			if(!value_to_word(curr)) /*converting the number into a word and checking if the converting was successful*/
				return FALSE;
			code_array[IC-100].A = 1; 
			IC++; 
		}
		else if(is_reg(token)) /*if the operand is a register*/
		{
			if(code_array[IC-100].opcode == 4) /*only lea can't get a register as an operand*/
			{
				return print_error("illegal operand");
			}	
			code_array[IC-100].source_addressing = 3;
			code_array[IC-100].source_reg = token[1]-48; /*-48 is the differance between a digit character to its ascii code*/ 
			code_array[IC-100].A = 1;
			IC++;
		}
		else if(*token == '&') /*if the operand is a label with relative addressing*/
		{
			return print_error("illegal operand"); /*source adressing can't be number 2*/ 
		}
		else /*if the operand is a label*/
		{
			if (!label_is_legal(token)) /*if the label isn't legal*/
			{
				return FALSE;
			}
			code_array[IC-100].source_addressing = 1;
			code_array[IC-100].A = 1;
			IC += 2;
		} 
		token = strtok(NULL, " ,\t\n"); /*getting the second operand*/
		if(!*token) /*if there is no second operand*/
			return print_error("too little operands");
		if(*token == '#') /*if the operand is a number*/
		{
			token++; 
			if(code_array[curr_IC-100].opcode != 1) /*only cmp can get a number as an operand*/
			{
				return print_error("illegal operand");
			}
			if(get_number(token) == TOO_BIG_NUMBER)  /*if the number is illegal*/
			{
				return print_error("illegal number in the operand");
			}
			curr = get_number(token);
			code_array[IC-100].A = 1; 
			if(!value_to_word(curr)) /*converting the number into a word and checking if the converting was successful*/
				return FALSE;
			IC++; 
		}
		else if(is_reg(token))/*if the operand is a register*/
		{
			code_array[curr_IC-100].dest_addressing = 3; 
			code_array[curr_IC-100].dest_reg = token[1]-48;  /*-48 is the differance between a digit character to its ascii code*/ 
		}
		else if(*token == '&') /*if the operand is a label with relative addressing*/
		{
			return print_error("illegal operand"); /*dest addressing can't be number 2*/ 
		}
		else /*if the operand is a label*/
		{
			if (!label_is_legal(token)) /*if the label is illegal*/
			{
				return FALSE;
			}
			code_array[curr_IC-100].dest_addressing = 1; 
			IC++;
		}
		token = strtok(NULL, " \t\n");
		if(token) /*if there is another word after the second operand*/
		{
			return print_error("too many operands");
		}
		return TRUE;
	}
}
		
			
			



	

  

















