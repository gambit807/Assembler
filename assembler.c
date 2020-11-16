#include "assembler.h"

int main(int argc, char *argv[])
{
  get_files(argc,argv); /*called to "get_files" function to start the program*/
  free_symbol_table(head); /*free the symbol table (that we allocated for it a memory during the program)*/
  return 0;
}
    
     
