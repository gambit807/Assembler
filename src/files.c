#include "assembler.h"

void get_files(int argc, char *argv[])
{
	FILE *input;
	int i = 1, j;
	char end[] = ".as";
	if (argc == 1) /*checks if there are no arguments in the command line*/
		printf("there were no files in the command line\n");
	for(;i<argc;i++)
	{
		strcpy(file_name,argv[i]);
		strcat(file_name,end); /*adds ".as" to the name of the argument*/
		input = fopen(file_name,"r"); /*opens the input file just for read*/
		if(!input)
			fprintf(stdout,"file %s can't be opened\n",file_name);
		else
		{
			if(pass_one(input)) /*creates the output files if there were no errors*/
			{
				ob_file(argv[i]);
				ent_file(argv[i]);
				ext_file(argv[i]);
			}
			fclose(input); /*closes the input file*/
        	}
		for(j=0;j < MAX_ARRAY_LEN; j++) /*resets the array*/
		{
			if(arrex[j].address == 0) /*the end of the array*/
				break;
			arrex[j].address = 0; 
		}
		for(j=0;j < MAX_ARRAY_LEN; j++) /*resets the array*/
		{
			if(!code_array[j].A && !code_array[j].R && !code_array[j].E) /*the end of the array*/
				break;
			code_array[j].A = 0;
			code_array[j].R = 0;
			code_array[j].E = 0;
			code_array[j].funct = 0;
			code_array[j].dest_reg = 0;
			code_array[j].dest_addressing = 0;
			code_array[j].source_reg = 0;
			code_array[j].source_addressing =0;
			code_array[j].opcode = 0;
		}
	}
}

FILE *create_file(char name[],char suffix[])
{
	FILE *f;
	strcpy(file_name,name);
	strcat(file_name,suffix); /*adds appropriate suffix to the name of the file*/
	f = fopen(file_name,"w"); /*opens file just for write*/
	return f;
}

void ob_file(char name[])
{
	int i;
	FILE *f = create_file(name,".ob"); /*creates the object file*/
	if(!f)
	{
		fprintf(stdout,"file %s can't be opened\n",file_name);
        	return; 
	}
	fprintf(f,"%d\t%d\n",(ICF - IC_START), DC); /*prints the code size and the data size*/
	for(i = 0;i < ICF-100;i++) /*first prints the code image*/
	{
		fprintf(f,"%07d\t%06x\n",IC_START+i, word_to_value(code_array[i]));
	}
	for (i = 0; i < DC; i++) /*then prints the data image*/
	{
		fprintf(f, "%07d\t%06lx\n", ICF + i, (data_array[i] & 0xffffff)); 
	}
}

void ent_file(char name[])
{
	int flag_entry = FALSE;
	symbol_ptr temp = head; /*contains at first the first element in the symbol tabel*/
	FILE *f;
	for(; temp != NULL; temp = temp->next) /*checks if there weren't entries at all*/
	{
		if(!strcmp(temp->type,"entry"))
		{
			flag_entry = TRUE; /*there was an entry*/
		}
	}
	if(flag_entry == FALSE)
	{
		return; /*shouldn't create an entry file if there weren't entries at all*/
	}
	f = create_file(name,".ent"); /*creates the entry file*/
	if(!f)
	{
		fprintf(stdout,"file %s can't be opened\n",file_name);
        	return;
    	}
	for(temp = head; temp != NULL; temp = temp->next) /*fills the entry file as required*/
	{
		if(!strcmp(temp->type,"entry"))
		{
			fprintf(f, "%s\t%07d\n", temp->symbol, temp->address);
		}
	}
}

void ext_file(char name[])
{
	int i;
	FILE *f;
	if(arrex[0].address == 0) /*checks if there wasn't externs at all*/
	{
		return; /*shouldn't create an extern file if there wasn't externs at all*/
	}
	f = create_file(name,".ext"); /*create the extern file*/
	if(!f)
	{
		fprintf(stdout,"file %s can't be opened\n",file_name);
        	return;
	}
	for(i=0; i<=MAX_ARRAY_LEN; i++) /*fills the extern file as required*/
	{
		if(arrex[i].address == 0) /*the end of the array*/
			break;
		fprintf(f, "%s\t%07d\n", arrex[i].label, arrex[i].address);
	}
}
	








