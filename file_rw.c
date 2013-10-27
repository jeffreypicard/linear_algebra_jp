/*
 * file_rw.c
 *
 * Reading and writing matrices to files.
 *
 * Matlab code for writing and reading matrices in files that
 * this program in compatible with.
 * 
 * dlmwrite('matrix.file',A,'delimiter',',')
 * B = dlmread('matrix.file',',')
 *
 * Author: Jeffrey Picard
 */

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "matrix_ops.h"

#include "file_rw.h"

matrix *
read_matrix( char *file_name )
{
  fprintf( stdout, "File to open, %s\n", file_name );
  FILE *file = fopen( file_name, "r");

  if ( !file ) {
    fprintf( stderr, "Error: could not open file.\n");
    return 0;
  }
  
  int column_count = 0;
  int columns = 0;
  int rows = 0;
  double num = 0;
  char c = 0;
  while( fscanf( file, "%lf%c", &num, &c ) != EOF ) {
    if ( c != '\n' ) {
      column_count++;
      //fprintf( stdout, "%.16lf%c", num, c );
      continue;
    }
    //fprintf(stdout, "%.16lf%c", num, c );
    column_count++;
    if ( columns == 0 )
      columns = column_count;

    //fprintf( stdout, "%d columns\n", column_count );
    column_count = 0;
    rows++;
  }
  //fprintf( stdout, "%d rows\n", rows );

  matrix *read_in = create_matrix( rows, columns );
  rewind( file );

  int row = 0;
  int column = 0;

  while( fscanf( file, "%lf%c", &num, &c ) != EOF ) {
    if ( c != '\n' ) {
      read_in->values[row][column] = num;
      column++;
      continue;
    }

    read_in->values[row][column] = num;

    column = 0;
    row++;
  }

  fclose( file );

  return read_in;
}

void
write_matrix( char *file_name, matrix *to_write )
{
  if ( !to_write ) {
    fprintf( stderr, "Error: matrix to write is null.\n");
    return;
  }

  FILE *file = fopen( file_name, "w");

  if ( !file ) {
    fprintf( stderr, "Error: could not open file for writing.\n");
    return;
  }
  
  int rows = to_write->rows;
  int columns = to_write->columns;

  for ( int row = 0; row < rows; row++ ) {
    for ( int column = 0; column < columns; column++ ) {
      if ( column == columns - 1 )
        fprintf( file, "%.16lf%c", to_write->values[row][column], '\n' );
      else
        fprintf( file, "%.16lf%c", to_write->values[row][column], ',' );
    }
  }

  fclose( file );
}

/*
 * Main program can be used to compile and run this
 * part of the program sperately for testing.
 */
#if MAIN
int main( int argc, char** argv )
{
  if ( argc < 2 ) {
    fprintf( stderr, "Error: No file argument.\n");
    exit( EXIT_FAILURE );
  }

  matrix *read_in = read_matrix( argv[1] );

  print_matrix( read_in );

  if ( argc == 3 ) {
    write_matrix( argv[2], read_in );
  }

  return 0;
}
#endif /* MAIN */
