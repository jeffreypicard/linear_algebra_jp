/*
 * prompt.h
 *
 * Header file for prompt.c
 *
 * Author: Jeffrey Picard
 */
#ifndef _PROMPT_H
#define _PROMPT_H
#include "matrix.h"

int is_sep( const char * );
int is_bin_op( const char * );
int is_unary_op( const char * );
int is_num(const char * );
int is_matrix_delim( const char * );

matrix *parse_matrix( char **, int, int );

int bound_matrix( char **, int );
int bound_paran( char **, int );
int bound_string( char**, int );

void print_welcome();
void print_prompt();
int tkn_inpt_length( char ** );
int execute_commands( char ** );
int parse_token_input( char ** );
int execute_commands_old( char ** );
int parse_raw_input( char * );
int get_input();

/*
 * Below here are test functions, not yet in use!
 *
 * Again, Jeff, comment about what the fuck you're doing...
 */
typedef struct parse_ret_struct {
  matrix * M;
  int index;
} parse_ret;


parse_ret *parse_matrix_test( char *input, int index );

#endif /* _PROMPT_H */
