TGT=relayer
SRC=Relay_Engine.c main.c
OBJ=Relay_Engine.o main.o
CC=gcc
CFLAGS+=-pthread
LDFLAGS+=-pthread

$(TGT):$(OBJ)
	$(CC)	$^ -o	$@	$(CFLAGS)	$(LDFLAGS)

clean:
	rm -rf $(OBJ) $(TGT)
