/************************************************************************
 * linear_algebra.c
 *
 * This program implements some linear algebra algorithms.
 *
 * Currently supported are lu decomposition, qr decomposition and
 * modified gram schmidt facoritization.
 *
 * In addition it provides an interactive prompt for easier use.
 *
 * Currently supported commands from the prompt.
 *
 * exit - exits the program.
 * tests - run a suite of tests.
 * read file variable - reads a matrix from a file in a format supported
 *                      by matlab. See file_rw.c for more detailed info.
 * write file variable - write a matrix to a file in a format supported
 *                       by matlab. See file_rw.c for more detailed into.
 *
 * Currently supported functions. (These differ from commands in that
 * they are built into a parse tree and evaluate to a matrix which is
 * either assigned to a given variable or to ans and printed out).
 *
 * matrix * matrix - does matrix multiplication.
 * ( matrix ) - parenthesis are treated as a function in the parse tree
 *              so it is listed here. Evaluated to the matrix.
 * lu ( matrix ) - does lu decomposition on the matrix, returns U.
 * qr ( matrix ) - does qr decomposition on the matrix, return Q.
 * mgs ( matrix ) - does modified gram schmidt decompostion on the matrix.
 *                  returns Q.
 *
 * Compilation command:
 *
 * gcc -std=c99 -g -Wall -lm -o linear_algebra linear_algebra.c matrix.c matrix_ops.c prompt.c parse_tree.c var_list.c tests.c file_rw.c
 *
 * Author: Jeffrey Picard
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define VAR_START_SIZE 10
#define VAR_NAME_SIZE 20

#include "matrix.h"
#include "var_list.h"
#include "matrix_ops.h"
#include "prompt.h"
#include "parse_tree.h"
#include "tests.h"
#include "file_rw.h"

/****************************************************** main ***********************************************************/

int main()
{
  print_welcome();

  while( 1 )
  {
    print_prompt();
    if( !get_input() )
      break;
  }
 
  delete_variables();
  printf("Normal termination.\n");
  return 0;
}
