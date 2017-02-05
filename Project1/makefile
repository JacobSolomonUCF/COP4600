_OBJ = fcfs.o rr.o sjf.o
OBJ = $(patsubst %, obj/%, $(_OBJ))

main: $(OBJ) obj/main.o
	gcc -o $@ $^

obj/%.o: %.c
	gcc -c $< -o $@

obj/main.o: main.c
	gcc -c $< -o $@
