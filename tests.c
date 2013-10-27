/*
 * tests.c
 *
 * Tests for linear_algebra_jp.
 *
 * TODO: Make all the values in results come out to 42 so I know
 *       they all worked.
 *
 * Author: Jeffrey Picard
 */

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "var_list.h"
#include "matrix_ops.h"
#include "prompt.h"
#include "parse_tree.h"
#include "tests.h"

void run_tests( void )
{
  /*matrix * z = create_matrix( 1000, 1000 );*/
  /*fill_random( z );*/

  /*multiplication tests*/

  matrix * m = create_matrix( 2, 2 );
  matrix * n = create_matrix( 2, 2 );

  m->values[0][0] = 1;
  m->values[1][0] = 2;
  m->values[0][1] = 1;
  m->values[1][1] = 2;

  print_matrix( m );

  n->values[0][0] = 1;
  n->values[1][0] = 2;
  n->values[0][1] = 1;
  n->values[1][1] = 2;

  print_matrix( n );

  matrix * mn = multiply( m, n );

  print_matrix( mn );

  matrix * a = create_matrix( 2, 3 );
  matrix * b = create_matrix( 3, 2 );

  a->values[0][0] = 1;
  a->values[1][0] = 2;
  a->values[0][1] = 1;
  a->values[1][1] = 2;
  a->values[0][2] = 1;
  a->values[1][2] = 2;

  print_matrix( a );

  b->values[0][0] = 1;
  b->values[1][0] = 2;
  b->values[2][0] = 1;
  b->values[0][1] = 1;
  b->values[1][1] = 2;
  b->values[2][1] = 1;

  print_matrix( b );

  matrix * ab = multiply( a, b );

  print_matrix( ab );

  /*Equal tests*/
  printf("------------------------------- Equal Tests "
         "-----------------------------------\n");

  printf("Equal Test 1:\n");
  print_matrix( ab );
  print_matrix( mn );
  printf("Result 1: %d\n\n", equal( ab, mn) );

  printf("Equal Test 2:\n");
  print_matrix( m );
  print_matrix( n );
  printf("Result 2: %d\n\n", equal( m, n) );

  printf("Equal Test 2:\n");
  print_matrix( m );
  print_matrix( m );
  printf("Result 2: %d\n\n", equal( m, m ) );

  /*Transpose tests*/
  printf("------------------------------- Transpose Tests "
         "-----------------------------------\n");

  matrix *ta = transpose( a );
  matrix *tb = transpose( b );
  matrix *tm = transpose( m );
  matrix *tn = transpose( n );

  printf("Matrix a\n");
  print_matrix( a );
  printf("Matrix a'\n");
  print_matrix( ta );

  printf("Matrix b\n");
  print_matrix( b );
  printf("Matrix b'\n");
  print_matrix( tb );

  printf("Matrix m\n");
  print_matrix( m );
  printf("Matrix m'\n");
  print_matrix( tm );

  printf("Matrix n\n");
  print_matrix( n );
  printf("Matrix n'\n");
  print_matrix( tn );

  /* Variable Tests*/
  printf("------------------------------- Variable Tests "
         "-----------------------------------\n");

  add_var( "m", m );
  add_var( "n", n );
  add_var( "a", a );
  add_var( "b", b );
  add_var( "ta", ta );
  add_var( "tb", tb );
  add_var( "tm", tm );
  add_var( "tn", tn );
  add_var( "ab", ab );
  add_var( "mn", mn );

  matrix *temp;

  if ( (temp = search_vars("ab")) )
    print_matrix( temp );
  if ( (temp = search_vars("C")) )
    print_matrix( temp );
  if ( (temp = search_vars("m")) )
    print_matrix( temp );

  delete_var("ab");

  if ( (temp = search_vars("ab")) )
    print_matrix( temp );
  if ( (temp = search_vars("C")) )
    print_matrix( temp );
  if ( (temp = search_vars("m")) )
    print_matrix( temp );

  /* Variable Tests*/
  printf("------------------------------- Function Tests "
         "-----------------------------------\n");

  matrix *zero1 = fill_zeros( create_matrix( 5, 5 ) );
  add_var( "zero1", zero1 );
  print_matrix( zero1 );

  matrix *copy1 = copy_matrix( zero1 );
  add_var( "copy1", copy1 );
  print_matrix( copy1 );

  matrix *lu_test_matrix = create_matrix( 2, 2 );
  add_var( "lu_test_matrix", lu_test_matrix );
  lu_test_matrix->values[0][0] = 1;
  lu_test_matrix->values[0][1] = 2;
  lu_test_matrix->values[1][0] = 3;
  lu_test_matrix->values[1][1] = 4;

  /*matrix * LUP;
  LUP = lu( lu_test_matrix, 0 );
  matrix * L = *LUP;
  matrix * U = *(LUP + 1);
  matrix * P = *(LUP + 2);
  add_var( "L", L );
  add_var( "U", U );
  add_var( "P", P );
  print_matrix( L );
  print_matrix( U );
  print_matrix( P );

  free( LUP );*/

  matrix *U = lu( lu_test_matrix, 1 );
  add_var( "U", U );
  print_matrix( U );

  /*matrix * P = fill_zeros( create_matrix( 3, 3 ) );
  add_var( "P", P );
  P->values[0][0] = 1;
  P->values[1][1] = 1;
  P->values[2][2] = 1;
  print_matrix( P );
  swap_rows( P, 0, 1 );
  print_matrix( P );*/

  puts("------------------------------------------------ QR Tests "
       "-----------------------------------------");

  matrix *qr_test_matrix = create_matrix( 2, 2 );
  add_var( "qr_test_matrix", qr_test_matrix );
  qr_test_matrix->values[0][0] = 1;
  qr_test_matrix->values[0][1] = 2;
  qr_test_matrix->values[1][0] = 3;
  qr_test_matrix->values[1][1] = 4;
  
  matrix *R = qr( qr_test_matrix );
  add_var( "R", R );
  print_matrix( R );

  puts("------------------------------------------------ "
       "Modified Gram Schimdt Tests "
       "-----------------------------------------");

  matrix *mgs_test_matrix = create_matrix( 2, 2 );
  add_var( "mgs_test_matrix", mgs_test_matrix );
  mgs_test_matrix->values[0][0] = 1;
  mgs_test_matrix->values[0][1] = 2;
  mgs_test_matrix->values[1][0] = 3;
  mgs_test_matrix->values[1][1] = 4;
  
  matrix *Q = mgs( mgs_test_matrix );
  add_var( "Q", Q );
  print_matrix( Q );
}
