CC = gcc
CFLAGS = -Og -mavx2
OBJS = 
TARGET = 
EXECUTABLE = float double prob2 prob3

all: $(EXECUTABLE)

float: float.o
	$(CC) $^ -o $@

float.o: float.c
	$(CC) -c float.c -o $@

double: double.o
	$(CC) $^ -o $@

double.o: double.c	
	$(CC) -c double.c -o $@


prob2: prob2.o float_prob2.o double_prob2.o
	$(CC) $(CFLAGS) $^ -o $@

prob2.o: prob2.c prob2.h
	$(CC) $(CFLAGS) -c prob2.c -o $@

float_prob2.o: float_prob2.c prob2.h
	$(CC) $(CFLAGS) -c float_prob2.c -o $@

float_prob2.s: float_prob2.c prob2.h
	$(CC) $(CFLAGS) -S float_prob2.c

double_prob2.o: double_prob2.c prob2.h
	$(CC) $(CFLAGS) -c double_prob2.c -o $@

double_prob2.s: double_prob2.c prob2.h
	$(CC) $(CFLAGS) -mavx2 -S double_prob2.c



prob3: prob3.o fma.o no_fma.o
	$(CC) -O2 -mavx2 $^ -o $@

prob3.o: prob3.c prob3.h
	$(CC) -O2 -mavx2 -c prob3.c -o $@	

fma.o: fma.c prob3.h
	$(CC) -O2 -mavx2 -mfma -c fma.c -o $@

fma.s: prob3.c prob3.h
	$(CC) -O2 -mavx2 -mfma -S fma.c

no_fma.o: no_fma.c prob3.h
	$(CC) -O2 -mavx2 -c no_fma.c -o $@

no_fma.s: no_fma.c prob3.h
	$(CC) -O2 -mavx2 -S no_fma.c



clean:
	-rm -f *.s *.o $(EXECUTABLE)
