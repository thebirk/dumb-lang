#include "cwc.h"

#define TOKEN_PLUS     1    // '+'
#define TOKEN_MINUS	   2    // '-'
#define TOKEN_RIGHT    3    // '>'
#define TOKEN_LEFT     4    // '<'
#define TOKEN_INPUT    5    // '?'
#define TOKEN_OUTPUT   6    // '!'
#define TOKEN_ZERO     7    // 'Z' or 'z'
#define TOKEN_WRITE    8    // 'W' or 'w'
#define TOKEN_READ     9    // 'R' or 'r'
#define TOKEN_READ_ADD 10   // 'A' or 'a'
#define TOKEN_READ_SUB 11   // 'S' or 's'

#define COMMENT_CHAR   ','

typedef struct{
	int size;
	int *tokens;
} TokenList;

defclass(Parser)
	char *src;
	char *path;
	int size;
	
	method(Parser, TokenList*, Parse)
endclass

decl_constructor(Parser, char* path)