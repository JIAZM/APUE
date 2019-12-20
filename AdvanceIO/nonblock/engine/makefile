TGT=relayer
SRC=Relay_Engine.c main.c
OBJ=Relay_Engine.o main.o
CC=gcc
LIBS=-lpthread

$(TGT):$(OBJ)
	$(CC)	$^ -o	$@	$(LIBS)

clean:
	rm -rf $(OBJ) $(TGT)
