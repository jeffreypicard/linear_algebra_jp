# Makefile taken from suckless style.

include config.mk

HEADERS =\
	file_rw.h\
	matrix.h\
	matrix_ops.h\
	parse_tree.h\
	prompt.h\
	var_list.h\
	tests.h

SRC =\
	linear_algebra.c\
	file_rw.c\
	matrix.c\
	matrix_ops.c\
	parse_tree.c\
	prompt.c\
	var_list.c\
	tests.c

OBJ = ${SRC:.c=.o}

all: options linear_algebra

options:
	@echo determinant build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"
	@echo "LD       = ${LD}"

%.o: %.c
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

linear_algebra: ${OBJ}
	@echo LD $@
	@${LD} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f linear_algebra ${OBJ} ${PROG_NAME}-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p ${PROG_NAME}-${VERSION}
	@cp -R Makefile LICENSE README config.mk ${SRC} ${HEADERS} ${PROG_NAME}-${VERSION}
	@tar -cf ${PROG_NAME}-${VERSION}.tar ${PROG_NAME}-${VERSION}
	@gzip ${PROG_NAME}-${VERSION}.tar
	@rm -rf ${PROG_NAME}-${VERSION}

.PHONY: all options clean dist
