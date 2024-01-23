%{
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

extern int yylineno; // Line number, provided by Lex
extern char *yytext; // The current token, provided by Lex

void yyerror(const char *format, ...) {
    va_list args;
    va_start(args, format); 

    fprintf(stderr, "line %d, near '%s': ", yylineno, yytext);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    
    va_end(args); 
}
 
int yywrap() {
    return 1;
} 

int yylex();
int yyparse(); 

int main() {
    yyparse();
} 

%}
			
%token PROCEDURE ENDP DECLARE

%union 
{
        int number;
        char *string;
}

%token <number> NUMBER
%token <string> ID

%%

/* start of temporary scaffolding */
commands: /* empty */
        | commands command
	| commands error ';' { yyerrok; } /* error recovery */
        ;

command:
        procedure 
        ;
/* end of temporary scaffolding */

parid:	'(' ID ')' 
        { printf("\tparid: %s\n", $2); }
        ;

params: '(' ID ')'
	{ printf("\tparams: %s\n", $2); }
	| '(' ID ',' ID ')'
	{ printf("\t2 params: %s and %s\n", $2, $4); }
	| '(' ID ',' ID ',' ID ')'      
	{ printf("\t3 params: %s, %s, and %s\n", $2, $4, $6); }
	;

declare: /* empty */
	| DECLARE declare_items ';'
	;

declare_items: declare_item 
	| declare_items ',' declare_item
	;

declare_item: ID
	{ printf("\tdeclare_item: %s\n", $1); }
	| ID '[' NUMBER ']'
	{ printf("\tdeclare_item %s[%d]\n", $1, $3); } 
	;

procedure: parid PROCEDURE params declare ENDP
	{ printf("\tPROCEDURE\n"); }
	;
