CC=gcc
CFLAGS=-Wall -Wextra -std=c99

grafos: main.c grafo.c particao.c bitvet.c algoritmos.c
	$(CC) $(CFLAGS) -o grafos main.c grafo.c particao.c bitvet.c algoritmos.c

saida: grafos
	./grafos > saida.txt

clean:
	rm -f grafos grafos.exe
