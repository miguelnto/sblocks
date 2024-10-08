include config.mk

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
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 ${BIN}/${MAIN} $(DESTDIR)$(PREFIX)/${MAIN}

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/${MAIN}

.PHONY: dirs all options install uninstall

