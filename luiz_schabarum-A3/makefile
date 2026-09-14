CC     = gcc
CFLAGS = -Wall -std=c11 -O2
LIBS   = -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lm

OBJS = main.o player.o perigos.o cenario.o interface.o

dinop: $(OBJS)
	$(CC) $(OBJS) -o dinop $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) dinop