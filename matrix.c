/*
 * matrix.c
 *
 * Code file for functions providing memory management
 * functions for a matrix.
 *
 * Author: Jeffrey Picard
 */
#include <stdlib.h>
#include "matrix.h"

matrix *
create_matrix( int rows, int columns )
{
  matrix *m = malloc( sizeof *m );
  m->rows = rows;
  m->columns = columns;
  m->values = malloc( sizeof *m->values * rows );

  for( int i = 0; i < rows; i++ )
    m->values[i] = malloc( sizeof **m->values * columns );
 
  return m;
}

void 
delete_matrix( matrix *m )
{
  if( !m )
    return;

  for( int i = 0; i < m->rows; i++ )
    free( m->values[i] );
  free( m->values );
  free( m );
}
