TGT=progressbar
SRC=progressbar.c
OBJ=progressbar.o

${TGT}:${OBJ}
	gcc	${OBJ} -o ${TGT}

${OBJ}:${SRC}
	gcc ${SRC} -c -o ${OBJ}

clean:
	@rm	${OBJ} ${TGT}
