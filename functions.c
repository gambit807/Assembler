#include "assembler.h"

int is_white(char ch)
{
  if(ch == ' ' || ch == '\t') /*checks if this character is a white space*/
    return TRUE;
  else 
    return FALSE;
}


int operands(char *op)
{
	int row;
	for(row = 0; row < 16; row++)
	{
		if(!strcmp(instructions[row][0],op)) /*checks if this string is one of the 16 operations*/
		{
			return atoi(instructions[row][3]); /*returns the operands number of this operation, if there was a match*/
		}
	}
	return print_error("the instruction is undefined"); /*prints an error message and returns FALSE, if there wasn't a match*/
}

int opcodes(char *op)
{
	int row;
	for(row = 0; row < 16; row++)
	{
		if(!strcmp(instructions[row][0],op)) /*checks if this string is one of the 16 operations*/
		{
			code_array[IC-100].opcode = atoi(instructions[row][1]);
			code_array[IC-100].funct = atoi(instructions[row][2]);
			operands_number = atoi(instructions[row][3]);
			return TRUE;
		}
	}
	return print_error("the instruction is undefined"); /*prints an error message and returns FALSE, if there wasn't a match*/
}
   

unsigned int word_to_value(word bitfield)
{
	unsigned int sum = 0;
	sum += bitfield.opcode<<18;
	sum += bitfield.source_addressing<<16;
	sum += bitfield.source_reg<<13;
	sum += bitfield.dest_addressing<<11;
	sum += bitfield.dest_reg<<8;
	sum += bitfield.funct<<3;
	sum += bitfield.A<<2;
	sum += bitfield.R<<1;
	sum += bitfield.E;
	return sum;
}

int value_to_word(int value)
{
	if (value >= TOO_BIG_NUMBER) /*if the number requires more than 21 bits*/
		return print_error("the number is too big");
	if(value < -TOO_BIG_NUMBER) /*if the number requires more than 21 bits*/
		return print_error("the number is too small");
	code_array[IC-100].funct = value & 31;
	value>>=5;
	code_array[IC-100].dest_reg = value & 7;
	value>>=3;
	code_array[IC-100].dest_addressing = value & 3;
	value>>=2;
	code_array[IC-100].source_reg = value & 7;
	value>>=3;
	code_array[IC-100].source_addressing = value & 3;
	value>>=2;
	code_array[IC-100].opcode = value & 63;
	return TRUE;
}


 



                            

