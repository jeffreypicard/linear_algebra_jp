/*
 * file_rw.h
 *
 * Header file for the functions to read and write matrices
 * to and from files in a form matlab can read.
 *
 * Author: Jeffrey Picard
 */

#ifndef _FILE_RW
#define _FILE_RW

matrix *read_matrix( char * );
void write_matrix( char *, matrix * );

#endif /* _FILE_RW */
