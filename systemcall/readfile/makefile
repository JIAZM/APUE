TGT = readmem
OBJ = readmem.o
SRCS = readmem.c

$(TGT) : $(OBJ)
	gcc $^ -o $@
$(OBJ) : $(SRCS)
	gcc -c -g $^ -o $@

clean :
	rm -rf $(TGT) $(OBJ)
