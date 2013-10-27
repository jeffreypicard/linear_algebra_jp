/*
 * matrix_ops.c
 *
 * Code file for matrix operations.
 *
 * Author: Jeffrey Picard
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "matrix.h"
#include "matrix_ops.h"

void
fill_random( matrix *m )
{
  /*clock_t start, end;*/

  /*start = clock();*/

  /*for ( int column = 0; column < m->columns; column++ )*/
    /*for ( int row = 0; row < m->rows; row++ )*/
      /*m->values[row][column] = rand();*/

  /*end = clock();*/

  /*printf("Clock ticks (column first): %u\n", (unsigned int) (end - start));*/

  /*start = clock();*/

  for ( int row = 0; row < m->rows; row++ )
    for ( int column = 0; column < m->columns; column++ )
      m->values[row][column] = rand();

  /*end = clock();*/

  /*printf("Clock ticks (row first): %u\n", (unsigned int) (end - start));*/
}

void
print_matrix( matrix *m )
{

  /*printf("Rows: %d\tColumns: %d\n", m->rows, m->columns );*/

  if ( !m ) {
    puts("Matrix null, nothing to print!");
    return;
  }

  for ( int row = 0; row < m->rows; row++ )
    for ( int column = 0; column < m->columns; column++ ) {
      printf("%.16f\t", m->values[row][column]);
      if ( column == m->columns - 1 )
        puts("");
    }

  puts("");
}

matrix *
multiply( matrix *a, matrix *b )
{
  if ( !a || !b ) {
    puts("Null in multiply.");
    return 0;
  }
  if ( (a->columns != b->rows) && 
       (a->columns != 1 || a->rows != 1) && 
       (b->columns != 1 || b->rows != 1) ) {
    puts("Size mismatch in multiply.");
    return 0;
  }

  if ( a->columns == 1 && a->rows == 1 ) {
    double num_multiply = a->values[0][0];
    matrix *result = create_matrix( b->rows, b->columns );
    for ( int row = 0; row < b->rows; row++ )
      for ( int column = 0; column < b->columns; column++ )
        result->values[row][column] = b->values[row][column] * num_multiply;
    return result;
  }

  if ( b->columns == 1 && b->rows == 1 ) {
    double num_multiply = b->values[0][0];
    matrix *result = create_matrix( a->rows, a->columns );
    for ( int row = 0; row < a->rows; row++ )
      for ( int column = 0; column < a->columns; column++ )
        result->values[row][column] = a->values[row][column] * num_multiply;
    return result;
  }

  matrix *result = create_matrix( a->rows, b->columns );
  for ( int row = 0; row < a->rows; row++ )
    for ( int column = 0; column < b->columns; column++ ) {
      double entry = 0;
      for ( int i = 0; i < a->columns; i++ )
        entry += a->values[row][i] * b->values[i][column];
      result->values[row][column] = entry;
    }

  return result;
}

/*
 * subtract
 *
 * This function calls do_add with subtract set to 1.
 */
matrix *
subtract( matrix *A, matrix *B )
{
  return do_add( A, B, 1 );
}

/*
 * add
 *
 * This function calls do_add with subtract set to 0.
 */
matrix *
add( matrix *A, matrix *B )
{
  return do_add( A, B, 0 );
}

/*
 * do_add
 * 
 * Arguments: A - matrix to be added to or subtracted from
 *            B - matrix to be added or subtracted
 *            subtract - int indicating whether to subtract or not.
 *
 * This function takes two matrices and an int either adds or subtracts
 * them based on the truth value of the int.
 */
matrix *
do_add( matrix *A, matrix *B, int subtract )
{
  if ( !A || !B ) {
    fprintf( stderr, "Error: In do_add, a matrix is null.\n");
    return 0;
  }
  if ( A->rows != B->rows || A->columns != B->columns ) {
    if ( A->rows == 1 && A->columns == 1 ) {
      matrix *C = create_matrix( B->rows, B->columns );
      fill_val( C, A->values[0][0] );
      /*int num_add = A->values[0][0];*/
      matrix * result = create_matrix( B->rows, B->columns );

      for ( int row = 0; row < B->rows; row++ )
        for ( int column = 0; column < B->columns; column++ )
          if ( subtract )
            result->values[row][column] = C->values[row][column] - B->values[row][column];
          else
            result->values[row][column] = C->values[row][column] + B->values[row][column];

      delete_matrix( C );
      return result;
    }

    if ( B->rows == 1 && B->columns == 1 ) {
      matrix *C = create_matrix( A->rows, A->columns );
      int num_add = B->values[0][0];

      for ( int row = 0; row < A->rows; row++ )
        for ( int column = 0; column < A->columns; column++ )
          if ( subtract )
            C->values[row][column] = A->values[row][column] - num_add;
          else
            C->values[row][column] = A->values[row][column] + num_add;

      return C;
    }    

    fprintf( stderr, "Error: In do_add matrix dimensions do not match.\n");
    return 0;
  }

  matrix *C = create_matrix( A->rows, A->columns );

  for ( int row = 0; row < A->rows; row++ )
    for ( int column = 0; column < A->columns; column++ )
      if ( subtract )
        C->values[row][column] = A->values[row][column] - B->values[row][column];
      else
        C->values[row][column] = A->values[row][column] + B->values[row][column];

  return C;
}

int
equal( matrix *a, matrix *b )
{
  if ( a->rows != b->rows || a->columns != b->columns )
    return 0;

  for ( int row = 0; row < a->rows; row++ )
    for ( int column = 0; column < a->columns; column++ )
      if ( a->values[row][column] != b->values[row][column] )
        return 0;

  return 1;
}

matrix *
copy_matrix( matrix *to_copy )
{
  if ( !to_copy )
    return 0;

  matrix *copy = create_matrix( to_copy->columns, to_copy->rows );
  for ( int row = 0; row < to_copy->rows; row++ )
    for ( int column = 0; column < to_copy->columns; column++ )
      copy->values[row][column] = to_copy->values[row][column];

  return copy;
}

matrix *
fill_zeros( matrix *a )
{
  if ( !a )
    return 0;

  for ( int row = 0; row < a->rows; row++ )
    for ( int column = 0; column < a->columns; column++ )
      a->values[row][column] = 0;

  return a;
}

matrix *
fill_val( matrix *a, double val )
{
  if ( !a )
    return 0;

  for ( int row = 0; row < a->rows; row++ )
    for ( int column = 0; column < a->columns; column++ )
      a->values[row][column] = val;

  return a;
}

/*
 * one_one
 *
 * Arguments: num - A double to make a 1x1 matrix from.
 *
 * This function takes a double and returns a 1x1 matrix
 * with it as its sole value. This is useful due to the
 * fact that all the matrix operations take only
 * matrices.
 */
matrix *
one_one( double num )
{
  matrix *A = create_matrix( 1, 1 );
  A->values[0][0] = num;

  return A;
}

matrix *
get_column( matrix *A, int column_to_get )
{
  if ( column_to_get < 0 || column_to_get >= A->columns ) {
    puts("Sir, you are silly.");
    return 0;
  }

  matrix *column_matrix = create_matrix( A->rows, 1 );
  for ( int x = 0; x < A->rows; x++ )
    column_matrix->values[x][0] = A->values[x][column_to_get];

  return column_matrix;
}

matrix *
get_row( matrix *A, int row_to_get )
{
  if ( row_to_get < 0 || row_to_get >= A->rows ) {
    fprintf( stderr, "Error: In get_row: Row requested not within matrix dimensions.\n");
    return 0;
  }

  matrix *row_matrix = create_matrix( 1, A->columns );
  for ( int x = 0; x < A->columns; x++ )
    row_matrix->values[0][x] = A->values[row_to_get][x];

  return row_matrix;
}

double
two_norm( matrix * a )
{
  double norm = 0;
  if ( a->columns != 1 && a->rows != 1 ) {
    fprintf( stderr, "Error: In two_norm: matrix is not a vector.\n");
    return -1;
  }
  if ( a->columns == 1 ) {
    for ( int x = 0; x < a->rows; x++ )
      norm += a->values[x][0] * a->values[x][0];

    return sqrt( norm );
  } else {
    for ( int x = 0; x < a->columns; x++ )
      norm += a->values[0][x] * a->values[0][x];

    return sqrt( norm );
  }
}

matrix *
transpose( matrix *a )
{
  matrix *b = create_matrix( a->columns, a->rows );

  for ( int row = 0; row < a->rows; row++ )
    for ( int  column = 0; column < a->columns; column++ )
      b->values[column][row] = a->values[row][column];
  
  return b;
}

void
swap_rows( matrix *a, int x, int y )
{
  if ( !a || a->rows < x || a->rows < y )
    return;

  double *temp = a->values[x];
  a->values[x] = a->values[y];
  a->values[y] = temp;
}

matrix *
lu( matrix *a, int verbose )
{
  if ( !a || a->columns != a->rows )
    return 0;

  matrix * P = fill_zeros( create_matrix( a->rows, a->columns ) );
  matrix * A = copy_matrix( a );
  matrix * L = fill_zeros( create_matrix( a->rows, a->columns ) );
  matrix * U = fill_zeros( create_matrix( a->rows, a->columns ) );
  int n = a->columns;

  for ( int x = 0; x < n; x++ ) {
    P->values[x][x] = 1;
    L->values[x][x] = 1;
  }

  for ( int x = 0; x < (n-1); x++ ) {
    /*L->values[ x ][ x ] = 1;*/
    
    /*if ( A->values[x][x] == 0 )*/
      /*continue;*/

    /*Apply permutations. */
    
    double cur_pivot = A->values[x][x];
    int swap = x;
    for ( int row = x+1; row < n; row++ ) {
      /*printf("Comparing for swap %f < %f\n", fabs(cur_pivot), 
       *        fabs(A->values[row][x]) );*/
      if ( fabs(cur_pivot) < fabs(A->values[row][x]) ) {
        cur_pivot = A->values[row][x];
        swap = row;
        /*printf("cur_pivot is now %f and the swap row is %d\n", 
         *        cur_pivot, swap );*/
      }
    }

    /* don't do elimination if the entire column is zero*/
    if ( A->values[swap][x] == 0 )
      continue;

    if ( swap != x ) {
      /*printf("Swapping %d and %d\n", swap, x);*/
      swap_rows( A, x, swap );
      swap_rows( P, x, swap );
    }

    for ( int row = x + 1; row < n; row++ ) {
      A->values[ row ][ x ] = A->values[ row ][ x ] / A->values[ x ][ x ];
      for ( int column = x + 1; column < n; column++ ) {
        A->values[ row ][ column ] = A->values[ row ][ column ] - 
                                     ( A->values[ row ][ x ] * 
                                       A->values[ x ][ column ] );
      }
    }
    if ( verbose >= 2 ) {
      printf("----------------------------------------\n");
      print_matrix( A );
      printf("----------------------------------------\n");
    }
  }

  for ( int row = 0; row < n; row++ )
    for ( int column = 0; column < n; column++ ) {
      if ( row > column )
        L->values[row][column] = A->values[row][column];
      else
        U->values[row][column] = A->values[row][column];
    }
  
  /*matrix ** to_return = malloc( sizeof(matrix*) * 3 );*/
  /**to_return = L;*/
  /**(to_return + 1) = U;*/
  /**(to_return + 2) = P;*/

  delete_matrix( A );

  if ( verbose ) {
    print_matrix( L );
    print_matrix( U );
    print_matrix( P );
  }

  delete_matrix( L );
  /*delete_matrix( U );*/
  delete_matrix( P );

  return U;
}

matrix *
qr( matrix *A )
{
  /*FIXME*/
  /*Unfortunately many matrices are needed to hold intermediate*/
  /*calculations or else memory will be leaked.*/
  /*Look into creating versions of functions that will do*/
  /*the needed operations in place when preservation of the*/
  /*original matrix is not essential.*/
  /*This should make the code more readable and increase performance.*/
  matrix *Q = fill_zeros( create_matrix( A->rows, A->columns ) );
  matrix *v = 0;

  matrix *v_t = 0;
  matrix *Q_j = 0;
  matrix *v_t_Q_j = 0;
  matrix *v_t_Q_j_Q_j = 0;
  /*matrix * A_i = 0;*/

  matrix *A_0 = get_column( A, 0 );
  double norm_A_column_one = two_norm( A_0 );
  delete_matrix( A_0 );
  for ( int i = 0; i < A->rows; i++ )
    Q->values[i][0] = A->values[i][0] / norm_A_column_one;

  for ( int i = 1; i < A->columns; i++ ) {
    v = get_column( A, i );
    /*print_matrix( A_i );*/
    /*print_matrix( Q );*/
    for ( int j = 0; j < i; j++ ) {
      /*printf("j = %d\n", j);*/
      
      /*v_t = transpose( v );*/
      matrix *A_i = get_column( A, i );
      v_t = transpose( A_i );
      delete_matrix( A_i );

      Q_j = get_column( Q, j );
      v_t_Q_j = multiply( v_t, Q_j );
      v_t_Q_j_Q_j = multiply( v_t_Q_j, Q_j );

      /*v = add( v, multiply( multiply( transpose( v ), get_column( Q, j ) ), 
       *         get_column( Q, j ) ), 1 );*/
      matrix *vNew = subtract( v, v_t_Q_j_Q_j );
      delete_matrix( v );
      v = vNew;

      delete_matrix( v_t );
      delete_matrix( Q_j );
      delete_matrix( v_t_Q_j );
      delete_matrix( v_t_Q_j_Q_j );
    }
    /*print_matrix( v );*/
    double v_norm = two_norm( v );
    /*printf("v_norm = %f\n", v_norm );*/
    for ( int z = 0; z < Q->rows; z++ )
      Q->values[z][i] = v->values[z][0] / v_norm;

    delete_matrix( v );
    /*delete_matrix( A_i );*/
  }

  return Q;
}

matrix *
mgs( matrix *A )
{
  int n = A->columns;
  matrix *v = copy_matrix( A );
  matrix *R = fill_zeros( create_matrix( A->rows, A->columns) );
  matrix *Q = fill_zeros( create_matrix( A->rows, A->columns) );

  /* Temp matrices so the memory can be deleted properly.*/
  matrix *v_column = 0;
  matrix *v_column_i = 0;
  matrix *v_column_j = 0;
  matrix *Q_column = 0;
  matrix *Q_column_t = 0;
  matrix *Qv = 0;

  matrix *R_i_j = 0;
  matrix *Q_i = 0;
  matrix *RQ = 0;

  for ( int i = 0; i < n; i++ ) {
    v_column_i = get_column( v, i );
    R->values[i][i] = two_norm( v_column_i );
    delete_matrix( v_column_i );
    for ( int j = 0; j < Q->rows; j++ )
      Q->values[j][i] = v->values[j][i] / R->values[i][i];

    for ( int j = i+1; j < n; j++ ) {
      Q_column = get_column( Q, i );
      Q_column_t = transpose( Q_column );
      v_column_j = get_column( v, j );
      Qv = multiply( Q_column_t, v_column_j );

      R->values[i][j] = Qv->values[0][0];

      R_i_j = one_one( R->values[i][j] );
      Q_i = get_column( Q, i );
      RQ = multiply( R_i_j, Q_i );

      v_column = subtract( v_column_j, RQ );
      /*v_column = add( v_column_j, multiply( one_one( R->values[i][j] ), get_column( Q, i ) ), 1 );*/
      for ( int k = 0; k < v->rows; k++ )
        v->values[k][j] = v_column->values[k][0];

      delete_matrix( v_column );
      delete_matrix( Q_column );
      delete_matrix( Q_column_t );
      delete_matrix( v_column_j );
      delete_matrix( Qv );

      delete_matrix( R_i_j );
      delete_matrix( Q_i );
      delete_matrix( RQ );
    }
  }

  /*
  matrix * test = multiply( Q, R );
  print_matrix( test );
  matrix * test2 = subtract( test, A );
  print_matrix( test2 );
  */

  /*delete_matrix( test2 );*/
  /*delete_matrix( test );*/
  delete_matrix( v );
  delete_matrix( R );

  return Q;
}
