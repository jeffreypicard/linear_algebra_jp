/*
 * prompt.c
 *
 * Interactive prompt for linear_algebra_jp
 *
 * Author: Jeffrey Picard
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "matrix.h"
#include "matrix_ops.h"
#include "prompt.h"
#include "var_list.h"
#include "parse_tree.h"
#include "file_rw.h"

void run_tests( void );
 
int
is_sep( const char *s )
{
  return strcmp( s, "|" ) == 0;
}

int
is_matrix_delim( const char *s )
{
  if ( strcmp( s, "[") == 0 )
    return 1;
  else if ( strcmp( s, "]") == 0 )
    return 2;
  else
    return 0;
}

int
is_bin_op_c( char c )
{
  switch( c ) {
    case '*':
    case '+':
    case '-':
      return 1;
    default:
      return 0;
  }
}

int
is_bin_op( const char *s )
{
  if ( (strcmp(s, "*") == 0) ||
      (strcmp(s, "+") == 0) ||
      (strcmp(s, "-") == 0) )
    return 1;
  return 0;
}

int
is_unary_op( const char *s )
{
  if ( (strcmp(s, "(") == 0) ||
      (strcmp(s, ")") == 0) ||
      (strcmp(s, "lu") == 0) ||
      (strcmp(s, "qr") == 0) ||
      (strcmp(s, "mgs") == 0) )
    return 1;
  return 0;
}

int
is_num(const char *s)
{
  if (s == NULL || *s == '\0' || isspace(*s))
    return 0;
  char *p;
  strtod(s, &p);
  return *p == '\0';
}

int
bound_matrix( char **input, int start )
{
  if ( ! is_num(input[start]) )
    return -1;

  int stop = start;
  while ( input[stop] && (is_num(input[stop]) || is_sep(input[stop])) )
    stop++;

  return stop;
}

int
bound_paran( char **input, int start )
{
  if ( strcmp(input[start], "(") != 0 )
    return -1;

  int stop_cond = 1;
  int stop = start + 1;
  while ( input[stop] && stop_cond != 0 ) {
    if ( strcmp(input[stop], ")") == 0 )
      stop_cond--;
    else if ( strcmp(input[stop], "(") == 0 )
      stop_cond++;
    stop++;
  }

  return stop;
}

matrix *
parse_matrix( char **commands, int start, int stop )
{
  /*printf("Start: %d Stop: %d\n", start, stop );*/

  float num = 0;
  int j = start;
  int column_track = 0;
  int rows = 0;
  int columns = 0;

  while ( j < stop && is_num( commands[j] ) ) {
    /*printf("commands[%d]: %s\n", j, commands[j]); */
    j++;
    column_track++;
  }

  columns = column_track;
  j = start;
  column_track = 0;
  rows++;
  while ( j < stop ) {
    column_track++;
    if ( ! is_num( commands[j] ) ) {
      if ( column_track - 1 != columns ) {
        printf("column_track: %d columns: %d\n", column_track, columns );
        return NULL;
      }
      column_track = 0;
      rows++;
    } else if ( j == (stop - 1) ) {
      if ( column_track != columns ) {
        printf("Lcolumn_track: %d columns: %d\n", column_track, columns );
        return NULL;
      }
    }
    j++;
  }

  /*printf("Parsed matrix is %d x %d\n", rows, columns );*/
  matrix *new = create_matrix( rows, columns );

  j = start;
  char *p;
  int row = 0;
  int column = 0;
  while ( j < stop ) {
    if ( column >= columns ) {
      row++;
      column = 0;
      j++;
    }
    num = strtof( commands[j++], &p );
    /*printf("%d x %d: %f\n", row, column, num );*/
    new->values[row][column] = num;
    column++;
  }

  return new;
}

void
print_welcome( void )
{
  printf("Hello!\n");
}

void
print_prompt( void )
{
  printf(">>> ");
}

int
tkn_inpt_length( char **input )
{
  int length = 0;
  while ( input[length] )
    length++;
  return length;
}

int
execute_commands( char **input )
{
  if ( strcmp( input[0], "exit") == 0 ) {
    if ( !input[1] ) {
      printf("\nGoodbye!\n");
      return 0;
    } else {
      printf("\nError: \"exit\" command takes no arguments.\n");
      return 1;
    }
  } else if ( strcmp( input[0], "tests" ) == 0 ) {
    if ( !input[1] )
      run_tests();
    else
      printf("\nError: \"tests\" command takes no arguments.\n");

    return 1;
  } else if ( strcmp( input[0], "read" ) == 0 ) {
    if ( !input[2] ) {
      printf("\nError: \"read\" command takes two arguments.\n");
    } else {
      /*printf("%s\n", input[1] );*/
      matrix *read_in = read_matrix( input[1] );
      if ( !read_in ) {
        printf("\nError: matrix file corrupted.\n");
        return 1;
      }
      add_var( input[2], read_in);
    }
    return 1;
  } else if ( strcmp( input[0], "write" ) == 0 ) {
    if ( !input[2] ) {
      fprintf( stderr, "\nError: In execute_commands: \"write\" "
                       "command takes two arguments.\n");
    } else {
      matrix * to_write = search_vars( input[2] );
      if ( !to_write ) {
        fprintf( stderr, "\nError: In execute commands: Specified "
                         "matrix is null.\n");
        return 1;
      }
      write_matrix( input[1], to_write );
    }
    return 1;
  } else if ( strcmp( input[0], "vars" ) == 0 ) {
    if ( input[1] ) {
      fprintf( stderr, "\nError: In execute_commands: \"vars\" command "
                       "takes no arguments.\n");
    } else {
      print_vars();
    }
    return 1;
  }
  else
    return -1;
}

int
parse_token_input( char **input )
{
  int length = tkn_inpt_length( input );
  int return_val = 0;

  /*printf("%d Tokens\n\n", length);*/
  /*printf("First Token: %s\n\n", input[0]);*/

  /* No input, return.*/
  if ( length == 0 ) {
    /*free( input );*/
    return 1;
  } else if ( (return_val = execute_commands( input )) >= 0 ) {
    /*Check for a string command and execute it if it exists.*/
    /*If it was executed, return it's return value.*/
    /*free( input );*/
    return return_val;
  } else if ( length == 1 ) {
    matrix *temp;
    if ( (temp = search_vars(input[0])) )
      print_matrix( temp );
    else
      printf("Unrecognized command, function, or variable %s.\n", input[0]);

    return 1;
  } else if ( input[1] && (strcmp( input[1], "=" ) == 0) ) {
    /*Check for assignment*/
    /*  parse tree, eval tree, assign, return*/
    if ( strlen(input[0]) >= NAME_LIMIT ) {
      printf("Error: variable name size must be under %d characters\n", 
             NAME_LIMIT );
      return 1;
    }
    matrix *var = 0;
    node *tree_root = 0;
    char *name = input[0];
    
    tree_root = create_parse_tree( input, 2, length );
    /*print_parse_tree( tree_root );*/
    
    var = eval_parse_tree( tree_root, 1 );
    delete_tree( tree_root );

    add_var( name, var );
    /*
    if ( search_vars( name ) )
    {
      delete_var( name );
      add_var( name, var );
    }
    else
    {
      add_var( name, var );
    }*/
    /*free( input );*/
    return 1;
  } else { 
    /*parse tree, eval tree, return*/
    /*printf("Input is expression, evaluating\n");*/
    matrix *var = 0;
    node *tree_root = 0;
    
    tree_root = create_parse_tree( input, 0, length );

    if ( !tree_root )
      printf("Tree root is null\n");

    /*print_parse_tree( tree_root );*/
    var = eval_parse_tree( tree_root, 1 );
    /*delete_var( "ans" );*/
    add_var( "ans", var );
    print_matrix( var );
    delete_tree( tree_root );
    /*delete_matrix( var );*/
    /*free( input );*/
    return 1;
  }
  /*return execute_commands_old( input );*/
}

int parse_raw_input( char * input )
{
  char **token_input = calloc(100, sizeof(char*));
  char *tok = strtok( input, " " );
  int j = 0;
  while ( tok ) {
    /*printf("Token %d: %s\n", j, tok);*/
    token_input[j++] = tok;
    tok = strtok( 0, " " );
  }

  /*free( input );*/
  int return_val = parse_token_input( token_input );
  free( token_input );
  return return_val;
}

int get_input( void )
{
  char c;
  char *input = calloc(100, sizeof(char));
  int j = 0;
  scanf("%c", &c);
  while ( c != '\n' && j < 100 ) {
    input[j++] = c;
    scanf("%c", &c);
  }
  /* If too much has been entered, clear the rest of stdin*/
  if ( j >= 100 ) {
    while ( c != '\n' )
      scanf("%c", &c);
    puts("Error: Input length exceeds 100 characters.");
    free( input );
    return 1;
  }

  int return_val = parse_raw_input( input );
  free( input );
  return return_val;
}
