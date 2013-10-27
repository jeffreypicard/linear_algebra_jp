/*
 * matrix.h
 *
 * Header file for matrix.c
 *
 * Author: Jeffrey Picard
 */
#ifndef _MATRIX_H
#define _MATRIX_H

typedef struct matrix_struct
{
  int rows;
  int columns;
  double ** values;
} matrix;

matrix *create_matrix( int, int );
void delete_matrix( matrix * );

#endif /* _MATRIX_H */
