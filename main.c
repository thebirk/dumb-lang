#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

void print_help()
{
	printf("Usage: dumbc <src>\n");
}

int main(int argc, char **argv)
{
	if(argc != 2) {
		print_help();
		return 0;
	}
	
	Parser *p = newParser(argv[1]);
	TokenList *list = p->Parse(p);
	
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
				if(memoffs >= 0) memoffs--;
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
		}
	}
	
	printf("\nFinished!\n");
	
	return 0;
}