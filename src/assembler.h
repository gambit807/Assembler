
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LABEL_LEN 31
#define MAX_LINE_LEN 81
#define IC_START 100
#define DC_START 0
#define NUM_OF_OPS 16
#define STR_EQUALS 0
#define MAX_ARRAY_LEN 1000
#define TOO_BIG_NUMBER 1048576
#define BIGGER_THAN_INT 8388608

enum bool{FALSE = 0, TRUE = 1};

extern int IC, DC, ICF, line_number, ex_index;
extern char file_name[MAX_ARRAY_LEN];

typedef struct symbol_table *symbol_ptr; /*a pointer to an element in the symbol table*/

struct symbol_table{ 
    char symbol[MAX_LABEL_LEN]; 
    int address; 
    char type[10]; 
    symbol_ptr next; 
};

typedef struct { /*the bitfield of every word in the code array*/
     unsigned int E : 1;
     unsigned int R : 1;
     unsigned int A : 1;
     unsigned int funct : 5;
     unsigned int dest_reg : 3;
     unsigned int dest_addressing : 2; 
     unsigned int source_reg : 3;
     unsigned int source_addressing : 2; 
     unsigned int opcode : 6;
} word;

extern symbol_ptr head;
extern word code_array[MAX_ARRAY_LEN]; /*the instruction image*/
extern long data_array[MAX_ARRAY_LEN]; /*the data image*/

typedef struct{ 
	char label[MAX_LABEL_LEN];
	int address;
} ext_struct;

extern ext_struct arrex[MAX_ARRAY_LEN];
extern char* instructions[16][4];
extern int operands_number; 


/*functions declarations*/

/*The function gets a file and does the first pass on it by sending line by line to the "line_parsing" function.
It returns true if the first pass didn't recognized errors, otherwise it returns false*/
int pass_one(FILE *);

/*The function gets a file and does the second pass on it by sending line by line to the "parse_two" function.
It returns true if the second pass didn't recognized errors, otherwise it returns false*/
int pass_two(FILE *);

/*The function gets a line from a file and analyzes it, by recognizing the type of the line (directive, code or data) and sending the operands to the needed function.
It returns true if the parse didn't find any errors, otherwise it returns false*/
int line_parsing(char *);

/*The function gets a string (with an error message), prints a detailed error message and returns false*/
int print_error(char *);

/*The function gets a string. it returns true if this string is a label (by definition), otherwise it returns false.*/
int is_label(char *);

/*The function gets a string. it returns true if this string is a register, otherwise it returns false.*/
int is_reg(char *);

/*The function gets a string. it returns true if this string is an operation, otherwise it returns false.*/
int is_op(char *);

/*The function gets a string. it returns true if this string is a directive, otherwise it returns false.*/
int is_directive(char *);

/*The function gets a string. it returns true if this string is a valid label, otherwise it returns false.*/
int label_is_legal(char *);

/*The function gets a string. it returns true if this string wasn't defined before, otherwise it returns false.*/
int multiple_label(char*);

/*The function gets a character. it returns true if this character is a space or a tab, otherwise it returns false.*/
int is_white(char);

/*The function gets a string that represent an operation. it returns how many opernads should be to this operation.
If this string is not one of the 16 operations, it prints an error message and returns false*/
int operands(char *);

/*The function gets a string that represent an operation. If this string is not one of the 16 operations, it prints an error message and returns false.
Otherwise it sets the opcode, funct and the operands number of this operation, and returns true*/
int opcodes(char *);

/*The function gets a word bitfield and returns it as an unsigned integer number*/
unsigned int word_to_value(word);

/*The function gets an integer number. If the number requires more than 21 bits, it returns false.
Otherwise it sets the number in the word bitfield as required and returns true.*/
int value_to_word(int);

/*The function gets an integer that represents the argc and a string that represents the argv (the command line arguments).
It opens the input files and creates the appropriate object, entry and extern files.*/ 
void get_files(int, char *argv[]);

/*The function gets two string that represents name and suffix. It adds appropriate suffix to the file's name,
and open this file just for write. It returns this open file.*/
FILE *create_file(char[], char[]);

/*The function gets a string that represents the file name (without a suffix). It creates the object file,
prints the code size and the data size, then prints the code image and then prints the data image*/
void ob_file(char[]);

/*The function gets a string that represents the file name (without a suffix). It creates the entry file (if there was an entry),
and fills the entry file as required*/
void ent_file(char[]);

/*The function gets a string that represents the file name (without a suffix). It creates the extern file (if there was an extern),
and fills the extern file as required*/
void ext_file(char[]);

/*The function gets a string that contains the operands in the instruction line, and adds words to the code array according to these operands.
It returns true if it doesn't find errors, otherwise it returns false*/
int instruction_line (char *);

/*The function gets a string, and enters all its numbers to the data array.
It returns true if it doesn't find errors, otherwise it returns false*/ 
int get_data(char *);

/*The function gets a string, and enters all its characters' ascii codes to the data array.
It returns true if it doesn't find errors, otherwise it returns false*/ 
int get_string(char *);

/*The function gets a string that should contain a number, converts it to a word and enters the word to the code array.
It returns true if it doesn't find errors, otherwise it returns false*/ 
int get_number(char *);

/*The function gets a line from a file and analyzes it. It fills the code array and the array of externs(arrex) with the final addresses of the labels. 
It returns true if the parse didn't find any errors, otherwise it returns false*/
int parse_two(char *);

/*The function gets the head of the symbol table. It frees the entire list (all the symbol table).*/
void free_symbol_table(symbol_ptr);

/*The function gets a string that represents the name of the symbol(label), an integer that represents the address, and string hat represents the type.
It creates a new node in the symbol tabel and adds it to the end of the list.*/ 
void add_symbol(char* name, int address, char* type);






















	
	
