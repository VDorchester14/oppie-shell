CC=gcc
CFLAGS=-I.
DEPS = signal_handlers.h job_manipulation.h
OBJ = oppie.o signal_handlers.o job_manipulation.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

oppie: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)