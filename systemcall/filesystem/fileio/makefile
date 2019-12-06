TGT = mycpy
OBJ = mycpy.o
SRCS = mycpy.c

$(TGT) : $(OBJ)
	gcc $^ -o $@
$(OBJ) : $(SRCS)
	gcc -c -g $^ -o $@

clean :
	rm -rf $(TGT) $(OBJ)
