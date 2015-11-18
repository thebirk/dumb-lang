#include "cwc.h"

#define TOKEN_PLUS       1    // '+'         Increment acc
#define TOKEN_MINUS      2    // '-'         Decrement acc
#define TOKEN_RIGHT      3    // '>'         Increment memoffs
#define TOKEN_LEFT       4    // '<'         Decrement memoffs
#define TOKEN_INPUT      5    // '?'         Input to acc
#define TOKEN_OUTPUT     6    // '!'         Output from acc
#define TOKEN_ZERO       7    // 'Z' or 'z'  Zero acc
#define TOKEN_WRITE      8    // 'W' or 'w'  Write acc to mem
#define TOKEN_READ       9    // 'R' or 'r'  Read from mem to acc
#define TOKEN_READ_ADD   10   // 'A' or 'a'  Add mem[memoffs] to acc
#define TOKEN_READ_SUB   11   // 'S' or 's'  Sub mem[memoffs] from acc
#define TOKEN_LOOP_START 12   // '['         Loop while mem[memoffs] is non-zero
#define TOKEN_LOOP_END   13   // ']'         
#define TOKEN_LOAD       14   // '%'         Set memoffs to acc
#define TOKEN_SAVE       15   // '/'         Write memoffs to acc

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