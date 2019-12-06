TGT = dup
OBJ = dup.o
SRCS = dup.c

$(TGT) : $(OBJ)
	gcc $^ -o $@
$(OBJ) : $(SRCS)
	gcc -c -g $^ -o $@

clean :
	rm -rf $(TGT) $(OBJ)
