#include "assembler.h"

int print_error(char *string)
{
	/*prints a detailed error message and returns FALSE*/
	printf("ERROR - file %s, line %d : %s\n",file_name, line_number,string);
	return FALSE; 
}
