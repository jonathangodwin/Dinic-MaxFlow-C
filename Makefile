CC = gcc
CFLAGS = -Wall -g
OBJ = main.o Graphe.o Sommet.o Chemin.o
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(EXEC)
