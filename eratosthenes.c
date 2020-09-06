//================================================================================================
// Soubor:      eratosthenes.c
// Řešení:      IJC-DU1, příklad a), 23. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Definice funkce Eratosthenes
// Poznámky:    Definovány jsou 2 funkce. 
//                  První je pro neoptimalizované pole, druhá pro optimalizované (viz bitset.h)
//================================================================================================

//================================================================================================
// File:        eratosthenes.c
// Case:        IJC-DU1, assignment a), 23. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: Definition of function Eratosthenes, which finds prime numbers using bitset array
// Notes:       There are deffined 2 functions
//                  First uses the non-optimized bitset array, second optimized (see bitset.h) 
//================================================================================================

#include "eratosthenes.h"

#ifndef OPTIMALIZATION  //not optimized

    void Eratosthenes(bitset_t pole)
    {
        if (pole == NULL)
            return;

        bitset_setbit(pole, 0, 1);
        bitset_setbit(pole, 1, 1);
        unsigned long sqrt_arr_size = (unsigned long)sqrt(bitset_size(pole));
        unsigned long arr_size = bitset_size(pole);

        for (unsigned long i = 2; i <= sqrt_arr_size; )
        {

            for (unsigned long j = i*i; j < arr_size; j += i)
                bitset_setbit(pole, j, 1);

            while(++i && bitset_getbit(pole, i));
        }
    }

#else   //optimized

    void Eratosthenes(bitset_t pole)
    {
        if (pole == NULL)
            return;
        // nubers 0, 1 and 2 share the same bit
        bitset_setbit(pole, 0, 1);

        //getting the sqrt of the array size, so the this function does not have to be called each cycle 
        unsigned long sqrt_arr_size = (unsigned long)sqrt(bitset_size(pole))<<1;
        
        unsigned long arr_size = bitset_size(pole);

        //unsigned long i = 3;
        for (unsigned long i = 3; i <= sqrt_arr_size; )
        {
            /*
            (i*i)>>1 == (i^2)/2 there is no need to set bits of smaller numbers than i^2 to true,
                (/2 is there because of the array dimension, as the array size is 2 times smaller then the acctual range)
            because these bits are already set to true (non prime) by smaller numbers than i.
            for example: i == 5; 10 is already set by 2, 15 by 3, 20 by. First not set is 25 == 5^2 

            j+=i
            adding the acctual number to j, which would be twice as big in array of normal size
            for example:
            i = 5; (j = (i*i)>>1) == 12 (actual number == 35), wchich is the index where the first bit is going to be set to 1
            then index 12+5=17 (actual number == 35) is going be set to 1, then 17+5=22 (in actual number == 45) 
            is going to be set to 1 , then...
            That is skipping the EVEN values, which cant't be prime numbers. (Thez have at least 2 as their divisor)

            Similar trick could be done on the array of the same range as the actual numbers, instead of 5 
            10 would have to be added each time.
            */

            for (unsigned long j = (i*i)>>1; j < arr_size; j+=i)
                bitset_setbit(pole, j, 1); 
            
            i>>=1;  //i/=2;
            //conversion to the array dimension 
            
            while(++i && bitset_getbit(pole, i)); 
            /* 
            Finding new prime number. It is not nessesary to check if 'i' is in the array size, 
            largest possible value of 'i' is always going to be smaller than */ 
            
            //bitset_getbit(pole, i++) does not work, beacuse 
            

            i = (i<<1) +1; //i = i*2+1;
            //conversion back to the real dimension (to an actual prime number)
        }
    }

#endif