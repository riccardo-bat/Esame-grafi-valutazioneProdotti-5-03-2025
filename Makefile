CC = gcc 
CFLAGS = -Wall

#generazione exe
esame: compito.o grafo.o tipo.o
	$(CC) $(CFLAGS) -o esame $^

#generazione oggetti
compito.o: compito.c grafo.h tipo.h 
	$(CC) $(CFLAGS) -c $^

grafo.o: grafo.c grafo.h
	$(CC) $(CFLAGS) -c $^

tipo.o: tipo.c tipo.h
	$(CC) $(CFLAGS) -c $^