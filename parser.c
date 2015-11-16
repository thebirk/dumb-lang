#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

TokenList *list_add(TokenList *list, int tok) {
	list->size++;
	list->tokens = realloc(list->tokens, sizeof(int)*list->size);
	list->tokens[list->size-1] = tok;
	
	return list;
}

decl_method(Parser, TokenList*, Parse)

defconstructor(Parser, char* path)
	self->path = path;

	FILE *f = fopen(path, "rb");
	if(!f) {
		printf("Failed to open '%s'\n", path);
		exit(EXIT_FAILURE);
	}
	
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);
	
	char *buffer = malloc(size*sizeof(char));
	int read = fread(buffer, sizeof(char), size, f);
	
	if(read != size) {
		printf("Error reading '%s'. Read %d out of %d bytes\n", path, read, size);
		fclose(f);
		exit(EXIT_FAILURE);
	}
	fclose(f);
	
	self->src = buffer;
	self->size = size;
	
	add_method(Parser, Parse)
endconstructor

defmethod(Parser, TokenList*, Parse)
	TokenList *list = new(TokenList);
	list->size = 0;
	list->tokens = NULL;
	
	int comments = 0;
	int i;
	for(i = 0; i < self->size; i++) {
		if(comments) {
			if(self->src[i] == COMMENT_CHAR) comments = 0;
			continue;
		}
		switch(self->src[i]) {
			case COMMENT_CHAR:
				comments = 1;
				continue;
				break;
			case '+':
				list = list_add(list, TOKEN_PLUS);
				break;
			case '-':
				list = list_add(list, TOKEN_MINUS);
				break;
			case '>':
				list = list_add(list, TOKEN_RIGHT);
				break;
			case '<':
				list = list_add(list, TOKEN_LEFT);
				break;
			case '?':
				list = list_add(list, TOKEN_INPUT);
				break;
			case '!':
				list = list_add(list, TOKEN_OUTPUT);
				break;
			case 'z':
			case 'Z':
				list = list_add(list, TOKEN_ZERO);
				break;
			case 'w':
			case 'W':
				list = list_add(list, TOKEN_WRITE);
				break;
			case 'r':
			case 'R':
				list = list_add(list, TOKEN_READ);
				break;
			case 'a':
			case 'A':
				list = list_add(list, TOKEN_READ_ADD);
				break;
			case 's':
			case 'S':
				list = list_add(list, TOKEN_READ_SUB);
				break;
				
			case '\n':
			case '\r':
			case '\t':
			case ' ':
				continue;
				break;
			
			default:
				printf("Unknown token '%c'\nHEX: 0x%02X\n", self->src[i], self->src[i]);
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	return list;
endmethod