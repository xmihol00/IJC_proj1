//================================================================================================
// Soubor:      primes.c
// Řešení:      IJC-DU1, příklad a), 25. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Soubor obsahující funkci main sloužící pro vypsání prvočísel.
//                  Program využívá bitsetové pole a funkce Erathosthenes.  
// Poznámky:    Obsahuje opět 2 funkce pro tisk prvočísel.
//                    1. pro neotimalizované pole, 2. pro optimalizované (viz bitset.h, eratosthenes.h)
//================================================================================================

//================================================================================================
// File:        eratosthenes.c
// Case:        IJC-DU1, assignment a), 23. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: File with a main function, which purpose is to write out prime numbers. 
//                  Program uses the bitset array and function Erathosthenes.
// Notes:       There are deffined 2 functions
//                  First uses the non-optimized bitset array, second optimized (see bitset.h, eratosthenes.h) 
//================================================================================================

#include <stdio.h>
#include <time.h>

#include "bitset.h"
#include "eratosthenes.h"

#define NUM_OF_PRINTED_PRIMES 10

// macros and declaration to test the sizes of arrays
#ifdef _SIZE_TESTS_
#define ALLOC_SIZE(velikost) ((((unsigned long)(velikost)-1)>>(4|(sizeof(unsigned long)>>2))) + 2)*sizeof(unsigned long)
#define ALLOC_SIZE_O(velikost) ((((((unsigned long)velikost)>>1)-1)/(sizeof(unsigned long)<<3)) + 2)*sizeof(unsigned long)
void test_sizes();
#endif

void primes_print(bitset_t pole);

int main()
{
    clock_t start = clock();

    bitset_create(pole, 500000000LU);
    Eratosthenes(pole);
    primes_print(pole);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);

    return 0;
}

#ifndef OPTIMALIZATION  // not optimized
void primes_print(bitset_t pole)
{
    if (pole == NULL)
        error_exit("primes_print: Pole neni alokováno.");

    long unsigned results_hold[NUM_OF_PRINTED_PRIMES] = {0,};

    for (unsigned long j = 0, i = bitset_size(pole); j<NUM_OF_PRINTED_PRIMES && --i>1; j++)
    {
        while(bitset_getbit(pole, i))
            i--;

        results_hold[j] = i;
    }

    for (long i = NUM_OF_PRINTED_PRIMES - 1; i >=0; i--)
        if(results_hold[i] != 0) 
            printf("%lu\n", results_hold[i]);

    return;
}

#else
void primes_print(bitset_t pole) // optimized
{
    if (pole == NULL)
        error_exit("primes_print: Pole neni alokováno.");

    long unsigned results_hold[NUM_OF_PRINTED_PRIMES] = {0,};

    for (unsigned long j = 0, i = bitset_size(pole); j<NUM_OF_PRINTED_PRIMES && --i>0; j++)
    {
        while(bitset_getbit(pole, i))
            i--;

        results_hold[j] = i;
    }

    if (!results_hold[NUM_OF_PRINTED_PRIMES-1])
        printf("2\n");
    /*
    printing 2 when the last index of of the array of results isn't filled
    (many more indexes before the last one can be also empty).
    That is because in the optimized version the prime number two isn't stored
    */

    for (long i = NUM_OF_PRINTED_PRIMES - 1; i >= 0; i--)
        if(results_hold[i] != 0)
            printf("%lu\n", (results_hold[i]<<1)+1);
    /*
    (results_hold[i]<<1)+1 == results_hold[i]*2+1
    increasing the value from the array dimension to the real number
    */
    return;
}
#endif





#ifdef _SIZE_TESTS_
#ifndef OPTIMALIZATION
void test_sizes()
{
    if (sizeof(long unsigned) == 8)
    {
        fprintf(stderr, "Kontrola velikostí polí ve 64-bit režimu.\n");

        bitset_create(p1, 127);
        if (bitset_size(p1) != 127 && sizeof(p1) != 3*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 127. sizeof je: %lu, měla být 24\n", bitset_size(p1), sizeof(p1));
        else
            fprintf(stderr, "p1: OK\n");
        
        bitset_create(p2, 128);
        if (bitset_size(p2) != 128 && sizeof(p2) != 3*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 128. sizeof je: %lu, měla být 24\n", bitset_size(p2), sizeof(p2));
        else
            fprintf(stderr, "p2: OK\n");

        bitset_create(p3, 129);
        if (bitset_size(p3) != 129 && sizeof(p3) != 4*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 129. sizeof je: %lu, měla být 32\n", bitset_size(p3), sizeof(p3));
        else
            fprintf(stderr, "p3: OK\n");

        bitset_create(p4, 130);
        if (bitset_size(p4) != 130 && sizeof(p4) != 4*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 130. sizeof je: %lu, měla být 32\n", bitset_size(p4), sizeof(p4));
        else
            fprintf(stderr, "p4: OK\n");

        bitset_create(p5, 623);
        if (bitset_size(p5) != 623 && sizeof(p5) != 11*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 623. sizeof je: %lu, měla být 88\n", bitset_size(p5), sizeof(p5));
        else
            fprintf(stderr, "p5: OK\n");

        if (ALLOC_SIZE(255) != 5*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 40\n", ALLOC_SIZE(255));
        else
            fprintf(stderr, "Alloc1: OK\n");
        
        if (ALLOC_SIZE(256) != 5*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 40\n", ALLOC_SIZE(256));
        else
            fprintf(stderr, "Alloc2: OK\n");
        
        if (ALLOC_SIZE(257) != 6*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 48\n", ALLOC_SIZE(257));
        else
            fprintf(stderr, "Alloc3: OK\n");

        if (ALLOC_SIZE(258) != 6*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 48\n", ALLOC_SIZE(258));
        else
            fprintf(stderr, "Alloc4: OK\n");
        
        if (ALLOC_SIZE(12) != 2*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 16\n", ALLOC_SIZE(12));
        else
            fprintf(stderr, "Alloc5: OK\n");

        if (ALLOC_SIZE(1000) != 17*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 136\n", ALLOC_SIZE(12));
        else
            fprintf(stderr, "Alloc6: OK\n");
    }
    else
    {
        fprintf(stderr, "Kontrola velikostí polí ve 32-bit režimu.\n");

        bitset_create(p1, 95);
        if (bitset_size(p1) != 95 && sizeof(p1) != 4*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 127. sizeof je: %u, měla být 16\n", bitset_size(p1), sizeof(p1));
        else
            fprintf(stderr, "p1: OK\n");
        
        bitset_create(p2, 96);
        if (bitset_size(p2) != 96 && sizeof(p2) != 4*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 128. sizeof je: %u, měla být 16\n", bitset_size(p2), sizeof(p2));
        else
            fprintf(stderr, "p2: OK\n");

        bitset_create(p3, 97);
        if (bitset_size(p3) != 97 && sizeof(p3) != 5*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 97. sizeof je: %u, měla být 20\n", bitset_size(p3), sizeof(p3));
        else
            fprintf(stderr, "p3: OK\n");

        bitset_create(p4, 98);
        if (bitset_size(p4) != 98 && sizeof(p4) != 5*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 98. sizeof je: %u, měla být 20\n", bitset_size(p4), sizeof(p4));
        else
            fprintf(stderr, "p4: OK\n");

        bitset_create(p5, 588);
        if (bitset_size(p5) != 588 && sizeof(p5) != 20*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 588. sizeof je: %u, měla být 80\n", bitset_size(p5), sizeof(p5));
        else
            fprintf(stderr, "p5: OK\n");

        if (ALLOC_SIZE(159) != 6*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 24\n", ALLOC_SIZE(159));
        else
            fprintf(stderr, "Alloc1: OK\n");
        
        if (ALLOC_SIZE(160) != 6*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 24\n", ALLOC_SIZE(160));
        else
            fprintf(stderr, "Alloc2: OK\n");
        
        if (ALLOC_SIZE(161) != 7*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 28\n", ALLOC_SIZE(161));
        else
            fprintf(stderr, "Alloc3: OK\n");

        if (ALLOC_SIZE(162) != 7*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 28\n", ALLOC_SIZE(162));
        else
            fprintf(stderr, "Alloc4: OK\n");
        
        if (ALLOC_SIZE(12) != 2*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 8\n", ALLOC_SIZE(12));
        else
            fprintf(stderr, "Alloc5: OK\n");

        if (ALLOC_SIZE(1000) != 33*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 132\n", ALLOC_SIZE(12));
        else
            fprintf(stderr, "Alloc6: OK\n");

        if (ALLOC_SIZE(256) != 9*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 36\n", ALLOC_SIZE(256));
        else
            fprintf(stderr, "Alloc7: OK\n");
        
        if (ALLOC_SIZE(257) != 10*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 40\n", ALLOC_SIZE(257));
        else
            fprintf(stderr, "Alloc8: OK\n");
    } 
}
#else
void test_sizes()
{
    if (sizeof(long unsigned) == 8)
    {
        fprintf(stderr, "Kontrola velikostí polí ve 64-bit režimu s optimalizací.\n");

        bitset_create(p1, 127);
        if (bitset_size(p1) != 63 && sizeof(p1) != 2*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 63. sizeof je: %lu, měla být 16\n", bitset_size(p1), sizeof(p1));
        else
            fprintf(stderr, "p1: OK\n");
        
        bitset_create(p2, 128);
        if (bitset_size(p2) != 64 && sizeof(p2) != 2*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 64. sizeof je: %lu, měla být 16\n", bitset_size(p2), sizeof(p2));
        else
            fprintf(stderr, "p2: OK\n");

        bitset_create(p3, 129);
        if (bitset_size(p3) != 64 && sizeof(p3) != 2*8)
            fprintf(stderr, "bitset_size je: %lu, měla být . sizeof je: %lu, měla být 16\n", bitset_size(p3), sizeof(p3));
        else
            fprintf(stderr, "p3: OK\n");

        bitset_create(p4, 130);
        if (bitset_size(p4) != 65 && sizeof(p4) != 3*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 130. sizeof je: %lu, měla být 24\n", bitset_size(p4), sizeof(p4));
        else
            fprintf(stderr, "p4: OK\n");

        bitset_create(p5, 623);
        if (bitset_size(p5) != 623 && sizeof(p5) != 6*8)
            fprintf(stderr, "bitset_size je: %lu, měla být 311. sizeof je: %lu, měla být 48\n", bitset_size(p5), sizeof(p5));
        else
            fprintf(stderr, "p5: OK\n");

        if (ALLOC_SIZE_O(255) != 3*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 24\n", ALLOC_SIZE_O(255));
        else
            fprintf(stderr, "Alloc1: OK\n");
        
        if (ALLOC_SIZE_O(256) != 3*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 24\n", ALLOC_SIZE_O(256));
        else
            fprintf(stderr, "Alloc2: OK\n");
        
        if (ALLOC_SIZE_O(257) != 3*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 24\n", ALLOC_SIZE_O(257));
        else
            fprintf(stderr, "Alloc3: OK\n");

        if (ALLOC_SIZE_O(258) != 4*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 32\n", ALLOC_SIZE_O(258));
        else
            fprintf(stderr, "Alloc4: OK\n");
        
        if (ALLOC_SIZE_O(12) != 2*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 16\n", ALLOC_SIZE_O(12));
        else
            fprintf(stderr, "Alloc5: OK\n");

        if (ALLOC_SIZE_O(1000) != 9*8)
            fprintf(stderr, "Alokovaná velikost je: %lu, měla být 72\n", ALLOC_SIZE_O(12));
        else
            fprintf(stderr, "Alloc6: OK\n");
    }
    else
    {
        fprintf(stderr, "Kontrola velikostí polí ve 32-bit režimu s optimalizací.\n");

        bitset_create(p1, 95);
        if (bitset_size(p1) != 47 && sizeof(p1) != 3*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 47. sizeof je: %u, měla být 12\n", bitset_size(p1), sizeof(p1));
        else
            fprintf(stderr, "p1: OK\n");
        
        bitset_create(p2, 96);
        if (bitset_size(p2) != 48 && sizeof(p2) != 3*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 48. sizeof je: %u, měla být 12\n", bitset_size(p2), sizeof(p2));
        else
            fprintf(stderr, "p2: OK\n");

        bitset_create(p3, 97);
        if (bitset_size(p3) != 48 && sizeof(p3) != 3*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 97. sizeof je: %u, měla být 12\n", bitset_size(p3), sizeof(p3));
        else
            fprintf(stderr, "p3: OK\n");

        bitset_create(p4, 98);
        if (bitset_size(p4) != 49 && sizeof(p4) != 5*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 49. sizeof je: %u, měla být 16\n", bitset_size(p4), sizeof(p4));
        else
            fprintf(stderr, "p4: OK\n");

        bitset_create(p5, 588);
        if (bitset_size(p5) != 294 && sizeof(p5) != 11*4)
            fprintf(stderr, "bitset_size je: %lu, měla být 588. sizeof je: %u, měla být 44\n", bitset_size(p5), sizeof(p5));
        else
            fprintf(stderr, "p5: OK\n");

        if (ALLOC_SIZE_O(191) != 4*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 16\n", ALLOC_SIZE_O(191));
        else
            fprintf(stderr, "Alloc1: OK\n");
        
        if (ALLOC_SIZE_O(192) != 4*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 16\n", ALLOC_SIZE_O(192));
        else
            fprintf(stderr, "Alloc2: OK\n");
        
        if (ALLOC_SIZE_O(193) != 4*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 16\n", ALLOC_SIZE_O(193));
        else
            fprintf(stderr, "Alloc3: OK\n");

        if (ALLOC_SIZE_O(194) != 5*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 28\n", ALLOC_SIZE_O(194));
        else
            fprintf(stderr, "Alloc4: OK\n");
        
        if (ALLOC_SIZE_O(12) != 2*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 8\n", ALLOC_SIZE_O(12));
        else
            fprintf(stderr, "Alloc5: OK\n");

        if (ALLOC_SIZE_O(1000) != 17*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 68\n", ALLOC_SIZE_O(1000));
        else
            fprintf(stderr, "Alloc6: OK\n");

        if (ALLOC_SIZE_O(256) != 5*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 20\n", ALLOC_SIZE_O(256));
        else
            fprintf(stderr, "Alloc7: OK\n");
        
        if (ALLOC_SIZE_O(257) != 5*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 20\n", ALLOC_SIZE_O(257));
        else
            fprintf(stderr, "Alloc8: OK\n");

        if (ALLOC_SIZE_O(258) != 6*4)
            fprintf(stderr, "Alokovaná velikost je: %u, měla být 24\n", ALLOC_SIZE_O(258));
        else
            fprintf(stderr, "Alloc9: OK\n");
    } 
}
#endif // end of optimalization
#endif // end of tests