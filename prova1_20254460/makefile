CC = gcc
CFLAGS = -std=c23 -Wall -Wextra
TARGET = prova1_20254460

all: $(TARGET)


$(TARGET): main.o arvoreB.o nodo.o
			$(CC)  $(CFLAGS) -o $(TARGET) main.o arvoreB.o nodo.o
main.o: main.c arvoreB.h 
			$(CC)  $(CFLAGS) -c main.c -o main.o
arvoreB.o: arvoreB.c arvoreB.h nodo.h
			$(CC)  $(CFLAGS) -c arvoreB.c -o arvoreB.o
nodo.o: nodo.c nodo.h arvoreB.h
			$(CC)  $(CFLAGS) -c nodo.c -o nodo.o	
			
clean:
		-rm -f $(TARGET) *.o