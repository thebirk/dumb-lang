#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*

Add support for loops inside loops

If arguments "-c" is present, then output a c program which can compile by itself

*/


void print_help()
{
	printf("Usage: dumbc <src>\n");
}

void find_loop(TokenList *list, int start, int *end) 
{
	int open = 0;
	
	int i;
	for(i = start; i < list->size; i++) {
		switch(list->tokens[i]) {
			case TOKEN_LOOP_START:
				open++;
				break;
			case TOKEN_LOOP_END:
				if(open > 0) {
					open--;
				}else {
					*end = i;
					return;
				}
				break;
		}
	}
}

void compile_list(TokenList *list)
{
	FILE *f = fopen("out.c", "w");
	fprintf(f, "#include <stdio.h>\n");
	fprintf(f, "#include <stdlib.h>\n");
	fprintf(f, "#include <stdint.h>\n");
	fprintf(f, "void main() {\n");
	
	fprintf(f, "uint16_t acc = 0;\n");
	fprintf(f, "uint16_t *mem = (uint16_t*)malloc(sizeof(uint16_t)*0x10000);\n");
	fprintf(f, "uint16_t memoffs = 0;\n");
	fprintf(f, "int val;\n");
	fprintf(f, "int ret;\n");
	
	int label_num = 0;
	int open = 0;
	
	int i;
	for(i = 0; i < list->size; i++) {
		switch(list->tokens[i]) {
			case TOKEN_PLUS:
				fprintf(f, "acc++;\n");
				break;
			case TOKEN_MINUS:
				fprintf(f, "acc--;\n");
				break;
			case TOKEN_RIGHT:
				fprintf(f, "if(memoffs < 0x10000) memoffs++;\n");
				break;
			case TOKEN_LEFT:
				fprintf(f, "if(memoffs > 0) memoffs--;\n");
				break;
			case TOKEN_INPUT:
				fprintf(f, "printf(\": \");\n");
				fprintf(f, "ret = scanf(\"%%d\", &val);\n");
				fprintf(f, "if(ret == EOF || ret == 0) {\nprintf(\"Invalid input!\\n\");\nexit(EXIT_FAILURE);\n}\n");
				fprintf(f, "acc = val;\n");
				break;
			case TOKEN_OUTPUT:
				fprintf(f, "printf(\"%%d\\n\", acc);\n");
				break;
			case TOKEN_ZERO:
				fprintf(f, "acc = 0;\n");
				break;
			case TOKEN_WRITE:
				fprintf(f, "mem[memoffs] = acc;\n");
				break;
			case TOKEN_READ:
				fprintf(f, "acc = mem[memoffs];\n");
				break;
			case TOKEN_READ_ADD:
				fprintf(f, "acc += mem[memoffs];\n");
				break;
			case TOKEN_READ_SUB:
				fprintf(f, "acc -= mem[memoffs];\n");
				break;
			case TOKEN_LOOP_START:
				fprintf(f, "L%d:\n", label_num++);
				open++;
				break;
			case TOKEN_LOOP_END:
				fprintf(f, "if(mem[memoffs] != 0){\n");
				fprintf(f, "goto L%d;\n}\n", label_num-open);
				open--;
				break;
			case TOKEN_LOAD:
				fprintf(f, "memoffs = acc;\n");
				break;
			case TOKEN_SAVE:
				fprintf(f, "acc = memoffs;\n");
				break;
		}
	}
	
	fprintf(f, "}\n");
	fclose(f);
}

int main(int argc, char **argv)
{
	int compile = 0;
	
	if(argc >= 3) {
		int i;
		for(i = 2; i < argc; i++) {
			if(strcmp(argv[i-1], "-c") == 0) {
				compile = 1;
			}
		}
	}else if(argc == 2) {
		compile = 0;
	}else {
		print_help();
		return 0;
	}
	
	Parser *p = newParser(argv[argc-1]);
	TokenList *list = p->Parse(p);
	
	if(compile == 1) {
		compile_list(list);
		return 0;
	}
	
	uint16_t acc = 0;
	int memoffs = 0;
	int memsize = 0x10000;
	uint16_t *mem = malloc(sizeof(uint16_t) * memsize);
	if(!mem) {
		printf("Failed to allocate program memory! Retrying with a lower value!\n");
		
		memsize = 0x8000;
		mem = malloc(0x8000);
		if(!mem) {
			printf("Failed to allocate program memory! Bye!\n");
			return -1;
		}
	}
	
	int i;
	for(i = 0; i < memsize; i++) {
		mem[i] = 0;
	}
	
	int loopstart = 0;
	int loopend = 0;
	
	for(i = 0; i < list->size; i++) {
		switch(list->tokens[i]) {
			case TOKEN_PLUS:
				acc++;
				break;
			case TOKEN_MINUS:
				acc--;
				break;
			case TOKEN_RIGHT:
				if(memoffs < memsize) memoffs++;
				break;
			case TOKEN_LEFT:
				if(memoffs > 0) memoffs--;
				break;
			case TOKEN_INPUT:
				printf(": ");
			
				int val;
				int ret = scanf("%d", &val);
				
				if(ret == EOF || ret == 0) {
					printf("Invalid input!\n");
					exit(EXIT_FAILURE);
				}
				
				acc = val;
				
				break;
			case TOKEN_OUTPUT:
				printf("%d\n", acc);
				break;
			case TOKEN_ZERO:
				acc = 0;
				break;
			case TOKEN_WRITE:
				mem[memoffs] = acc;
				break;
			case TOKEN_READ:
				acc = mem[memoffs];
				break;
			case TOKEN_READ_ADD:
				acc += mem[memoffs];
				break;
			case TOKEN_READ_SUB:
				acc -= mem[memoffs];
				break;
			case TOKEN_LOOP_START:
				loopstart = i;
				find_loop(list, loopstart, &loopend);
				break;
			case TOKEN_LOOP_END:
				if(mem[memoffs] != 0) {
					i = loopstart;
				}
				break;
			case TOKEN_LOAD:
				memoffs = acc;
				break;
			case TOKEN_SAVE:
				acc = memoffs;
				break;
		}
	}
	
	printf("\nFinished!\n");
	
	return 0;
}