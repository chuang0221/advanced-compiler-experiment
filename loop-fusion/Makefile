CC = gcc
CFLAGS = -O1

all: fused non-fused

fused: fused.c
	$(CC) $(CFLAGS) fused.c -o fused

non-fused: non-fused.c
	$(CC) $(CFLAGS) non-fused.c -o non-fused

clean:
	rm fused non-fused
