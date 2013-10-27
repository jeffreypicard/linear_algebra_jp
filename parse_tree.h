/**************************************************************
 * parse_tree.h
 *
 * Header file for parse_tree.c
 *
 * Author: Jeffrey Picard
 *************************************************************/

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "matrix.h"

enum node_type { BINARY_OP, UNARY_OP, VALUE };

typedef struct tree_node
{
  struct tree_node * parent;
  struct tree_node * left;
  struct tree_node * right;
  enum node_type type;
  // Stores the node's operation if it is an operator node.
  // Functions are treated as operators so it is set to a
  // reasonable size.
  char op[30];
  matrix * value;
  // This is set to 1 if the variable in the tree is also
  // stored in the programs var_list (memory). This keeps the
  // parse tree's delete code from deleting the variable
  // since it is a pointer to the same element stored in
  // the memory and will be deleted from there at an 
  // appropriate time.
  int in_memory;
} node;


void print_parse_tree( node * );

node * create_unary_op_node( char *);

node * create_val_node( matrix *, int );

node * create_bin_op_node( char *);

node * add_bin_op( node *, char *);

node * add_unary_op( node *, node *);

node * add_val( node *, matrix *, int );

node * create_parse_tree( char **, int, int );
matrix * eval_parse_tree( node *, int );

void delete_tree( node * );

#endif
