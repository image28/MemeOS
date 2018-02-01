#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Varables
#define UCHAR unsigned char
#define ULONG unsigned long
#define UINT  unsigned int

// SWITCHES
#define MAX 1000

// PRINTING
#define CHECK printf(".")
#define EL printf("\n")

#define MSG(text) printf("%s ",text); EL
#define ERR(text) printf("ERROR: %s ",text); EL

// MAIN
#define MAIN int main(int argc, char *argv[]) {

// ARGUMENTS
#define ARG_CHECK(ARGS) if ( argc < (ARGS+1) ) { ERR("Incorrect number of arguments"); exit(-1); }
#define ARGCHECK(ARGS) if ( argc < (ARGS+1) ) { ERR("Incorrect number of arguments"); exit(-1); }

// FUNCTIONAL
#define RET return(0)

// REQUIRED VARABLES
#define INPUT_FILE FILE *input;
#define OUTPUT_FILE FILE *output
#define FILES unsigned char inbyte,outbyte;
#define OUTFILE(PREFIX) unsigned char outfile[MAX]; strcpy(outfile,argv[1]); strcat(outfile,PREFIX)

// FILE IO
#define OPEN(FILE,NAME,MODE) if ( ( FILE = fopen(NAME,"MODE") ) == NULL ) printf("FILE file could not open"); exit(-2)
#define CLOSE(FILE) fclose(FILE)
#define READ fread(&inbyte,1,1,input)
#define WRITE fwrite(&outbyte,1,1,output)
#define FILELOOP while( ! feof(input) ) {
#define RESET_FILE fseek(input,0L,SEEK_SET)
#define SEEK(amount) fseek(input,amount,SEEK_SET)
#define FILESIZE fseek(input,0L,SEEK_END); filesize=ftell(input); fseek(input,0L,SEEK_SET)

// LOOPS
#define END }

