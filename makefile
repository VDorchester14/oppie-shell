CC = gcc
CFLAGS = -Wall -g
OBJ = oppie.o

all: oppie

oppie: $(OBJ)
	$(CC) $(CFLAGS) -o oppie $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
