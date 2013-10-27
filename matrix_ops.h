/*
 * matrix_ops.h
 *
 * Header file for matrix_ops.c
 *
 * Author: Jeffrey Picard
 */
#ifndef _MATRIX_OPS_H
#define _MATRIX_OPS_H

void fill_random( matrix * );
void print_matrix( matrix * );
matrix *multiply( matrix *, matrix * );
int equal( matrix *, matrix * );
matrix *copy_matrix( matrix * );
matrix *fill_zeros( matrix * );
matrix *fill_val( matrix *, double );
matrix *one_one( double );
matrix *get_column( matrix *, int );
matrix *get_row( matrix *, int );
matrix *do_add( matrix *, matrix *, int );
matrix *add( matrix *, matrix * );
matrix *subtract( matrix *, matrix * );
double two_norm( matrix * );
matrix *transpose( matrix * );
void swap_rows( matrix *, int, int );
matrix *lu( matrix *, int );
matrix *qr( matrix * );
matrix *mgs( matrix * );

#endif /* _MATRIX_OPS_H */
