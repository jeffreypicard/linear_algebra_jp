/********************************************************************
 * var_list.h
 *
 * Header file for var_list.c.
 * This file contains the global variable needed for the variable
 * mechanism.
 *
 * Author: Jeffrey Picard
 **********************************************************************/
#ifndef VAR_LIST_H
#define VAR_LIST_H
#define NAME_LIMIT 20

typedef struct variable_struct
{
  char name[NAME_LIMIT];
  matrix * data;
  struct variable_struct * link;
} variable;

// Global variable head that starts the linked list of all variables
extern variable * head;

matrix * search_vars( char * );

void add_var( char *, matrix * );

void delete_var( char *);

void delete_variables();

void print_vars();

#endif
