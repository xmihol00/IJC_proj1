#================================================================================================
# Soubor:      Makefile
# Řešení:      IJC-DU1, příklad a) b), 20. 2. 2020
# Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
# Popis:       Makefile pro vytvoření spustitelných souborů primes, primes-i, steg-decode a steg-encode
# Poznámky:	   Přepínač -OPTIMALIZATION je určen k optimalizování bitsetového pole pro větší
#				 efektivitu hledání prvočísel
#			   Po jakékoli změně Makefile je nutné všechny souboru znovu přeložit (příkaz" make remake)
#================================================================================================

#================================================================================================
# File:        Makefile
# Case:        IJC-DU1, assignment a) b),  20. 2. 2020
# Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
# Description: Makefile, which creates executable files: primes, primes-i, steg-decode a steg-encode 
# Notes:       Switch -DOPTIMALIZATION is used to optimize the bitset array for better efficiency
# 			      in finding the prime numbers:x
#                  First uses the non-optimized bitset array, second optimized (see bitset.h, eratosthenes.h)
# 			   It is necessary to recompile ALL the files after any change of the Makefile (command: make remake) 
#================================================================================================

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g -O2 #-DOPTIMALIZATION #-m32 #-Werror #-fsanitize=address
LDFLAGS = -lm #-fsanitize=address
DEPS = bitset.h error.h eratosthenes.h			#dependences on primes and eratosthenes
DEPS-S = ppm.h error.h bitset.h eratosthenes.h 	#dependences on steg
DEPS-B = bitset.h error.h						#dependences on bitset
OBJ = primes.o error.o eratosthenes.o bitset.o	#objects for primes
OBJ-I = primes-i.o eratosthenes-i.o bitset-i.o error.o #objects for primes-i
OBJ-SD = steg-decode.o ppm.o error.o eratosthenes.o bitset.c #objects for steg-decode
OBJ-SE = steg-encode.o ppm.o error.o eratosthenes.o bitset.c #objects for steg-encode
EXE = primes primes-i steg-decode steg-encode 	#executables

all: $(EXE)

primes: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o primes

primes-i: $(OBJ-I)
	$(CC) $(OBJ-I) $(CFLAGS) $(LDFLAGS) -o primes-i

steg-decode: $(OBJ-SD)
	$(CC) $(OBJ-SD) $(CFLAGS) $(LDFLAGS) -o steg-decode

steg-encode: $(OBJ-SE)
	$(CC) $(OBJ-SE) $(CFLAGS) $(LDFLAGS) -o steg-encode

error.o: error.c error.h
	$(CC) error.c $(CFLAGS) -c -o error.o

ppm.o: ppm.c ppm.h error.h 
	$(CC) ppm.c $(CFLAGS) -c -o ppm.o

steg-decode.o: steg-decode.c $(DEPS-S)
	$(CC) steg-decode.c $(CFLAGS) -c -o steg-decode.o

steg-encode.o: steg-encode.c $(DEPS-S)
	$(CC) steg-encode.c $(CFLAGS) -c -o steg-encode.o

bitset.o: bitset.c $(DEPS-B)
	$(CC) bitset.c $(CFLAGS) -c -o bitset.o

bitset-i.o: bitset.c $(DEPS-B)
	$(CC) bitset.c $(CFLAGS) -DUSE_INLINE -c -o bitset-i.o

eratosthenes.o: eratosthenes.c $(DEPS)
	$(CC) eratosthenes.c $(CFLAGS) -c -o eratosthenes.o

eratosthenes-i.o: eratosthenes.c $(DEPS)
	$(CC) eratosthenes.c $(CFLAGS) -DUSE_INLINE -c -o eratosthenes-i.o

primes.o: primes.c $(DEPS)
	$(CC) primes.c $(CFLAGS) -c -o primes.o

primes-i.o: primes.c $(DEPS)
	$(CC) primes.c $(CFLAGS) -DUSE_INLINE -c -o primes-i.o

.PHONY: clean
clean:
	`rm *.o $(EXE)` || true   
# || true --> the cleaning procedure alwayse ends with success, even when some files don't exist.
# It needs to be there so command remake would work

.PHONY: run
run: primes primes-i
	ulimit -s 75000; ./primes
	ulimit -s 75000; ./primes-i

.PHONY: remake
remake: clean $(EXE)

.PHONY: archive
archive:
	zip xmihol00 *.c *.h Makefile README.txt
