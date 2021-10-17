CC = gcc
CFLAGS = -Wall -g -pg -ansi
LD = gcc
LDFLAGS = -g -pg

all: htable main.o huffman.o

htable: huffman.o main.o
	$(LD) $(LDFLAGS) -o htable huffman.o main.o
main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c
huffman.o : huffman.c huffman.h
	$(CC) $(CFLAGS) -c -o huffman.o huffman.c
clean: htable
	rm huffman.o main.o
test: htable
	./htable st.txt > stOut.txt
	~pn-cs357/demos/htable st.txt > stSol.txt
	diff -s stOut.txt stSol.txt
testman: htable
	./htable /usr/share/man/*/* > outme.txt
	~pn-cs357/demos/htable /usr/share/man/*/* > outsol.txt
	diff -s outme.txt outsol.txt
testwap: htable
	./htable WAP.txt > WAPO.txt
	~pn-cs357/demos/htable WAP.txt > WAPI.txt
	diff -s WAPO.txt WAPI.txt
testone: htable
	./htable oc > ocOut.txt
	~pn-cs357/demos/htable oc > ocSol.txt
	diff -s ocOut.txt ocSol.txt
debug: htable
	gdb htable
valgrind: htable
	valgrind -s --leak-check=full --show-leak-kinds=all ./htable st.txt
