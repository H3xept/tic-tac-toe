CC = gcc 
CCFLAGS = -Wall -pedantic
OUT = ttt

all:
	$(CC) $(CCFLAGS) *.c -o $(OUT)

clean:
	rm -rf *.o
