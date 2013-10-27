/***************************************************************
 * var_list.c
 *
 * Code file for the variable linked list used by the linear
 * algebra program.
 *
 * Author: Jeffrey Picard
 **************************************************************/
#include "matrix.h"
#include "matrix_ops.h"
#include "var_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

variable * head = 0;

matrix * search_vars( char * var_name )
{
  variable * walk = head;
  while( walk )
  {
    if( strcmp( walk->name, var_name ) == 0 )
      return walk->data;

    walk = walk->link;
  }

  return 0;
}

void add_var( char * var_name, matrix * data )
{
  //variable * temp = head;
  variable * temp_var = 0;
  matrix * temp_val = 0;
  if( (temp_val = search_vars( var_name ))  )
  {
    if( data == temp_val )
      return;
    else
      delete_var( var_name );
  }

  temp_var = head;
  variable * new = malloc(sizeof(variable));

  strcpy( new->name,  var_name);
  new->data = data;
  new->link = temp_var;
  head = new;
}

void delete_var( char * var_name )
{
  variable * walk = head;
  variable * prev = walk;
  while( walk )
  {
    if( strcmp( walk->name, var_name ) == 0 )
    {
      if( walk == head )
      {
        puts("Deleting head variable.");
        variable * temp = head;
        head = head->link;
        delete_matrix( temp->data );
        free( temp );
      }
      else
      {
        puts("Deleting non-head variable.");
        prev->link = walk->link;
        delete_matrix( walk->data );
        free( walk );
      }
      return;
    }
    prev = walk;
    walk = walk->link;
  }
}

void delete_variables()
{
  //print_vars();
  variable * walk = head;
  while( walk )
  {
    variable * to_delete = walk;
    walk = walk->link;
    delete_matrix( to_delete->data );
    free( to_delete );
  }

  head = 0;
}

void print_vars()
{
  variable * walk = head;
  while( walk )
  {
    printf("%s\n", walk->name );
    print_matrix( walk->data );
    walk = walk->link;
  }
}
