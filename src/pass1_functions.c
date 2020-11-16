#include "assembler.h"

int get_data(char *data)
{
	int sign = 1, comma_flag = -1;
	long curr = 0;
	char comma = ',';
	if(data == NULL) /*if there is nothing after the ".data"*/
	{
		return print_error("there aren't any numbers in the data");
	}   
	while(*data)
	{
		if(is_white(*data))
		{	
			data++;
		}
		else if(isdigit(*data))
		{
			if(!comma_flag) /*if there wasn't comma before the number and this is not the first number*/
			{
				return print_error("There is a missing comma");
			}
			while(isdigit(*data)) /*getting the whole number*/
			{
				curr = curr*10+(*data)-48; /*-48 is the differance between a digit character to its ascii code*/ 
				data++;	
			}
			curr *= sign;
			if (curr >= BIGGER_THAN_INT)
				return print_error("too big number");
			if (curr < -BIGGER_THAN_INT)
				return print_error("too small number");
			data_array[DC] = curr; /*entering the number into the data array*/
			DC++;
			comma_flag = FALSE;
			sign = 1;
		}
		else if(*data == '+' || *data == '-') 
		{	
			if(!data[1] || !isdigit(data[1]) || isdigit(data[-1])) /*if the sign doesn't come before a number*/
			{
				return print_error("the sign is not before the number");
			}
			if(*data == '-')
				sign = -1; /*saving the sign for the number that comes after*/
			data++;
		}
		else if(*data == comma)
		{
			if(comma_flag == -1) /*if there were no numbers before*/
			{
				return print_error("the data can't start with a comma");
			}
			else if(comma_flag) /*if the last non-white character was a comma*/
			{
				return print_error("there are multiple of commas");
			}
			else
			{
				comma_flag = TRUE;
				data++;
				curr = 0;
			}
		}
		else /*if the character is not a number, not a comma and not white*/
		{
			return print_error("data doesn't have only numbers and commas");
		}
				
	}
	if(comma_flag == -1) /*if there were no numbers*/
		return print_error("there aren't any numbers in the data");
	if(comma_flag) /*if the last non-white character was a comma*/
		return print_error("data can't end with a comma");
	return TRUE;	
}

int get_string(char *str)
{    
	char *end;
	while (is_white(*str)) /*getting the start of the string*/
	{	
		str++;
	} 
	if(*str != '"') /*if the first non-white character isn't a comma*/
	{
		return print_error("this is not a string");
	}
	end = str;
	while(*end) /*end points at the end of line*/
	{
		end++;
	} 
	end--;
	while (is_white(*end)) /*end points at the end of the string*/
	{	
		end--;
	} 
	if(*end != '"' || end == str) /*if the last non-white character isn't a comma*/
	{
		return print_error("this is not a string");
	}
	str++;
	while(str!=end) /*getting the whole string*/
	{
		data_array[DC] = (long)*str; /*entering the char's ascii code into the data array*/
		DC++;
		str++;
	}
	data_array[DC] = '\0'; /*entering the char's ascii code into the data array*/
	DC++;
	return TRUE;
}


int get_number(char *token)
{
	int curr = 0, sign = 1;
	if(*token == '+' || *token == '-') /*if the first character is a sign*/
	{
		if(*token == '-')
			sign = -1;
		token++;
	}
	if(isdigit(*token)) 
	{
		while(isdigit(*token))/*getting the whole number*/
		{
			curr = curr*10+(*token)-48;
			token++;	
		}
		if(*token) /*if there is another character after the digits*/
		{
			return TOO_BIG_NUMBER;
		}
		return curr * sign;
	}
	
	return TOO_BIG_NUMBER;/*if the first character is not a digit nor a sign*/
}
