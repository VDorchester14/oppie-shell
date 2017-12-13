CC=gcc
CFLAGS=-I.
DEPS = signal_handlers.h
OBJ = oppie.o signal_handlers.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

oppie: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)