/*
 * parse_tree.c
 *
 * Implements a parse tree for arbitrary length expressions in
 * linear_algebra_jp. This should really be replaced by something
 * more robust like flex/bison.
 *
 * Author: Jeffrey Picard
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "matrix_ops.h"
#include "parse_tree.h"
#include "prompt.h"
#include "var_list.h"
#include "file_rw.h"

/*
 * print_parse_tree
 *
 * Arguments: root - root node of the tree to print.
 *
 * Takes a node and prints the parse tree in an extremely simple manner.
 * Use is very limited.
 */
void
print_parse_tree( node *root )
{
  if( !root )
    return;

  switch( root->type ) {
    case BINARY_OP:
      puts("B");
      break;
    case UNARY_OP:
      puts("U");
      break;
    case VALUE:
      puts("V");
      break;
    default:
      break;
  }
  if( root->left )
    print_parse_tree( root->left );
  if( root->right )
    print_parse_tree( root->right );
}

/*
 * create_unary_op_node
 *
 * Arguments: op - string represtenting the operation.
 *
 * Takes a string representing a unary operation and create and returns
 * an associated node to be used in the parse tree.
 */
node *
create_unary_op_node( char *op )
{
  node *new_node = malloc( sizeof *new_node );
  new_node->type = UNARY_OP;

  new_node->parent = 0;
  new_node->left = 0;
  new_node->right = 0;

  strcpy( new_node->op, op );
  new_node->value = 0;
  new_node->in_memory = 0;

  return new_node;
}

node *
create_val_node( matrix *value, int in_memory )
{
  node *new_node = malloc( sizeof *new_node );
  new_node->type = VALUE;

  new_node->parent = 0;
  new_node->left = 0;
  new_node->right = 0;

  new_node->value = value;

  new_node->in_memory = in_memory;

  return new_node;
}

node *
create_bin_op_node( char *op )
{
  node *new_node = malloc( sizeof *new_node );
  new_node->type = BINARY_OP;

  new_node->parent = 0;
  new_node->left = 0;
  new_node->right = 0;

  strcpy( new_node->op, op );
  new_node->value = 0;
  new_node->in_memory = 0;

  return new_node;
}

node *
add_bin_op( node *root, char *op )
{
  if( !root ) {
    return root;
  } else if( root->type == UNARY_OP ) {
    root->left = add_bin_op( root->left, op );
    /* Expression passed unary op was bad
     * return null */
    if( !root->left )
      return 0;

    return root;
  }
 
  node *new_node = create_bin_op_node( op );

  new_node->left = root;
  new_node->right = 0;
  root->parent = new_node;

  return new_node;
}

node *
add_unary_op( node *root, node *op )
{
  if( !root ) {
    return op;
  } else if( root->type == VALUE ) {
    return 0;
  } else if( root->type == UNARY_OP ) {
    /* FIXME */
    /* Fuck you, Jeff. Put a sensible comment with your FIXMEs*/
    root->left = add_unary_op( root->left, op );
    if( !root->left )
      return 0;

    return root;
  } else {
    root->right = op;
    return root;
  }
}

node *
add_val( node *root, matrix *value, int in_memory )
{
  if( !root ) {
    node *new_node = create_val_node( value, in_memory );
    return new_node;
  } else if( root->type == VALUE ) {
    return 0;
  } else if( root->type == UNARY_OP ) {
    root->left = add_val( root->left, value, in_memory );
    if( !root->left )
      return 0;

    return root;
  } else {
    node *new_node = create_val_node( value, in_memory );
    root->right = new_node;
    return root;
  }
}

node *
create_parse_tree( char **input, int start, int end)
{
  /* puts("Creating parse tree");
   * printf("Start: %d End: %d\n", start, end); */
  node *root = 0;
  matrix *m = 0;

  int j = start;
  while( j < end ) {
    if( (m = search_vars(input[j])) ) {
      /*printf("Variable found in memory, adding to tree.\n");*/
      if( !(root = add_val( root, m, 1 )) )
        return 0;
      j++;
    } else if( is_num(input[j]) ) {
      /*printf("Matrix expression found, parsing\n");*/
      int stop = bound_matrix( input, j );
      m = parse_matrix( input, j, stop );
      if( !m ) {
        fprintf(stderr, "Error: In create_parse_tree: Bad matrix expression.\n");
        return 0;
      } else {
        if( !(root = add_val( root, m, 0 )) ) {
          fprintf(stderr, "Error: In create_parse_tree: Illegal tree add.\n");
          return 0;
        }
        j = stop;
      }
    } else if( is_bin_op(input[j]) ) {
      /*printf("Binary operator found, adding to tree\n");*/
      if( !(root = add_bin_op( root, input[j] )) )
        return 0;
      j++;
    } else if( is_unary_op(input[j]) ) {
      /*printf("Unary operator found, adding to tree\n");*/
      if( strcmp(input[j], "(") == 0 ) {
        node *u = create_unary_op_node( input[j] );
        int stop = bound_paran( input, j );
        node *val = create_parse_tree( input, j+1, stop );
        
        if( !val )
          return 0;

        u->left = val;
        val->parent = u;

        /*print_parse_tree( u );*/

        root = add_unary_op( root, u );

        /*print_parse_tree( root );*/

        j = stop;
      } else if( strcmp( input[j], ")") == 0 ) {
        return root;
      }
      else if( strcmp( input[j], "lu") == 0 || 
               strcmp( input[j], "qr") == 0 || 
               strcmp( input[j], "mgs") == 0 ) {
        if( strcmp( input[j+1], "(") != 0 ) {
          fprintf(stderr, "Error: In create_parse_tree: Use paranthesis when "
                          "calling functions to avoid ambiguity!\n");
          return 0;
        }
        node *u = create_unary_op_node( input[j] );
        int stop = bound_paran( input, j+1 );
        node *val = create_parse_tree( input, j+2, stop );

        if( !val )
          return 0;
        
        u->left = val;
        val->parent = u;
        root = add_unary_op( root, u );
        
        /*printf("j is now: %d\n", j);*/
        j = stop;
      }
    } else {
      fprintf(stderr, "Error: In create_parse_tree: Expression \"%s\" is "
                      "not valid command, variable, or function.\n", 
                      input[j] );
      return 0;
    }
  }

  return root;
}

/*
 * eval_parse_tree
 * Arguments: root - root node of the parse tree to evaluate.
 *            assignment - integer indicating whether the result of
 *              the evaluation will be assigned into a variable.
 *
 * This function takes a root node of a parse tree and an integer
 * indicitating assignment or not and recursively evaluates the
 * parse tree, ultimately returning a matrix expression.
 */
matrix *
eval_parse_tree( node *root, int assignment )
{
  matrix *v = 0;
  if( !root )
    return v;
  
  if( root->type == VALUE ) {
    if( assignment && (root->parent == 0 || 
        strcmp( root->parent->op, "(" ) == 0 ) ) {
      if( root->in_memory == 1 ) {
        puts("variable already in memory, returning copy.");
        return copy_matrix( root->value );
      } else {
        root->in_memory = 1;
      }
    }
    return root->value;
  } else if( root->type == BINARY_OP ) {
    if( strcmp( root->op, "*" ) == 0 )
      return multiply( eval_parse_tree( root->left, 0 ), 
                       eval_parse_tree( root->right, 0 ) );
    else if( strcmp( root->op, "+") == 0 )
      return add( eval_parse_tree( root->left, 0 ), 
                  eval_parse_tree( root->right, 0 ) );
    else if( strcmp( root->op, "-") == 0 )
      return subtract( eval_parse_tree( root->left, 0 ), 
                       eval_parse_tree( root->right, 0 ) );
    else
      return v;
  } else if( root->type == UNARY_OP ) {
    if( strcmp( root->op, "(") == 0 )
      return eval_parse_tree( root->left, assignment );
    else if( strcmp( root->op, "lu") == 0 )
      return lu( eval_parse_tree( root->left, assignment ), 1 );
    else if( strcmp( root->op, "qr") == 0 )
      return qr( eval_parse_tree( root->left, assignment ) );
    else if( strcmp( root->op, "mgs") == 0 )
      return mgs( eval_parse_tree( root->left, assignment ) );
    else
      return v;
  } else {
    return v;
  }
}

/*
 * delete_tree
 * Arguments: root - root node of the tree to be deleted.
 *
 * This function takes the root node of a parse tree and
 * recursively deletes the entire tree, also removing any
 * matrices stored in the tree during parsing and computation
 * unless they are tagged as being variables stored in
 * memory.
 */
void
delete_tree( node *root )
{
  if( !root )
    return;

  if( root->value && !root->in_memory ) {
      puts("Node has value and not in memory, deleting\n");
      delete_matrix( root->value );
  }
  if( root->left )
    delete_tree( root->left );
  if( root->right )
    delete_tree( root->right );

  free( root );
}
