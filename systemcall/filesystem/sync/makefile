TGT = sync
OBJ = sync.o
SRCS = sync.c

$(TGT) : $(OBJ)
	gcc $^ -o $@
$(OBJ) : $(SRCS)
	gcc -c -g $^ -o $@

clean :
	rm -rf $(TGT) $(OBJ)
