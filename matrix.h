/***************************************************************
 * matrix.h
 *
 * Header file for matrix.c
 *
 * Author: Jeffrey Picard
 **************************************************************/
#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix_struct
{
  int rows;
  int columns;
  double ** values;
} matrix;

matrix * create_matrix( int, int );

void delete_matrix( matrix * );

#endif
