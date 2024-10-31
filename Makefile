include config.mk

SRCDIR = src
BIN = bin
SRC = ${SRCDIR}/*.c

all: dirs options ${MAIN} 

dirs:
	mkdir -p bin

options:
	@echo ${MAIN} build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC = ${CC}"

${MAIN}: 
	${CC} ${CFLAGS} ${LDFLAGS} ${SRC} -o ${BIN}/$@ 

install: all
	mkdir -p ${DESTDIR}${INSTALLDIR}
	install -m 0755 ${BIN}/${MAIN} ${DESTDIR}${INSTALLDIR}/${MAIN}

uninstall:
	rm -f ${DESTDIR}${INSTALLDIR}/${MAIN}

.PHONY: dirs all options install uninstall
