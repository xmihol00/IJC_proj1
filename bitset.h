//================================================================================================
// Soubro:      bitset.h
// Řešení:      IJC-DU1, příklad a), 20. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008 
// Popis:       Makra a definice inline funkcí definující operace nad bitovým polem
// Poznámky:    Soubor obsahuje 2 sety maker.
//                  První vytváří pole o normální velikosti, druhý vytváři optimalizované pole
//                      pro výpočet prvočísel.
//                  Druhý set potřebuje 2x méně paměti a počítá asi 2.25x rychleji.
//================================================================================================

//================================================================================================
// File:        bitset.h
// Case:        IJC-DU1, assignment a), 20. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: Macros and definitions of inline functions for the use of bitset array 
// Notes:       File contains two sets of Macros
//                  First creates a bitset array of normal size, second creates optimized array
//                      for primes numbers calculation.
//              Second set uses half the memory and is around 2.25x faster.                
//================================================================================================

#ifndef _BITSET_H
#define _BITSET_H

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"


#define LSB_MASK_DELETION ~1LU // == 0xFFFFFFFFFFFFE == 2^64-2 or 1111 ... 1110 in binary

#define CHAR_BIT 8
#define CHAR_BIT_SHIFT 3
 
//Data types
typedef unsigned long* bitset_t;
typedef unsigned long bitset_index_t;

extern unsigned long free_layover; 
/*
Variable declared to hold the size of an dynamically allocated array, that has to be deallocated before calling error_exit.
This would be implemented differently, if there wasn't specified in the assignment that error_exit has to be called when 
indexation error occures.
*/

//Macros
#ifndef OPTIMALIZATION     // ===MACROS SET 2=== not optimized

    #define bitset_create(jmeno_pole, velikost)\
    unsigned long jmeno_pole[((((unsigned long)(velikost)-1)>>(4|(sizeof(unsigned long)>>2))) + 2)] = {((unsigned long)(velikost))<<1, 0,};\
    static_assert((velikost)> 0L, "bitset_create: Chyba při překladu");\
    static_assert((((velikost)>>((sizeof(unsigned long)-4)*2)) <= 0x7FFFFFFFL), "Velikost pole je mimo implementační omezení.")
    /*
    To avoid division and modulo
    ((velikost-1)>>(4|(sizeof(unsigned int)>>2)) == ((velikost-1)/(sizeof(unsigned int)*8)
    */

    #define bitset_alloc(jmeno_pole, velikost)\
    assert((velikost) > 0L && (((velikost)>>((sizeof(unsigned long)-4)*2)) <= 0x7FFFFFFFL));\
    bitset_t jmeno_pole = calloc(((((unsigned long)(velikost)-1)>>(4|(sizeof(unsigned long)>>2))) + 2), sizeof(unsigned long));\
    if (jmeno_pole == NULL)\
        error_exit("bitset_alloc: Chyba alokace paměti");\
    jmeno_pole[0] = (((unsigned long)(velikost)<<1) + 1)

    /*I am using the LSB of jmeno_pole[0] to indicate the type of allocation. In this case the LSB is set to 1, to indicate
      that the array was created dynamically. 
      The implementation limit of the array is set to 2e9 for 32-bit architectures, so there is no possible way,
      that the unsigned long would overflow. On 32-bit architecture UL can hold value up to 2^32-1,which is more than 2e9<<1(4e9).
      On 64-bit architectures the limit is set to 2^39-1(maximum)
    */      

    /*
    To avoid division and modulo
    (((velikost)>>(2|(sizeof(unsigned long)>>3)))+(sizeof(unsigned long)<<((velikost&(sizeof(unsigned long)-1))&&1)))
    == (velikost/sizeof(unsigned long) + ((((velikost%(sizeof(unsigned long)))&&1) +1)*sizeof(unsigned long)))
    */

#else  //===MACROS SET 2=== optimized

    /*
    ===Optimalization explanation===
    There is no need to check if EVEN numbers are prime numbers (appart form 2), beacuse they have at least one divisor (number 2),
    It is possible to shrink the size of of the created array by half and then calculate with it appropriately.
    each index of the array now represents a number calculated by this function: f(index) = 2*index + 1
                                                                             or: f(index) = (index<<1) + 1 
                                                                             (which I believe is faster)
    This optimalization speeds up the calculation 2.25 times and uses half of the memory
    */

    #define bitset_create(jmeno_pole, velikost)\
    unsigned long jmeno_pole[((((((unsigned long)velikost)>>1)-1)/(sizeof(unsigned long)<<3)) + 2)] = {(((unsigned long)(velikost)) & LSB_MASK_DELETION), 0,};\
    static_assert(((velikost) > 2L), "bitset_create: Chyba při překladu");\
    static_assert((((velikost) >>((sizeof(unsigned long)-4)*2)) <= 0x7FFFFFFFL), "Velikost pole je mimo implementační omezení.");

    #define bitset_alloc(jmeno_pole, velikost)\
    assert((velikost) > 2L && (((velikost)>>((sizeof(unsigned long)-4)*2)) <= 0x7FFFFFFFL));\
    bitset_t jmeno_pole =\
    calloc(((((((unsigned long)velikost)>>1)-1)/(sizeof(unsigned long)<<3)) + 2), sizeof(unsigned long));\
    if (jmeno_pole == NULL)\
        error_exit("bitset_alloc: Chyba alokace paměti");\
    jmeno_pole[0] = (((unsigned long)(velikost) & LSB_MASK_DELETION) + 1) // +1 sets the LSB

    /*
    I am using the LSB of jmeno_pole[0] to indicate the type of allocation. In this case the LSB is set to 1, to indicate
    that the array was created dynamically. In order to be able to free it later.
    The implementation limit of the array is set to 2^31-1(maximum) for 32-bit architectures, so there is no possible way,
    that the unsigned long would overflow. On 32-bit architecture UL can hold value up to 2^32-1.
    On 64-bit architectures the limit is set to 2^39-1(maximum)
    */ 
                    
#endif //end of optimalization option


#ifndef USE_INLINE
    
    #define bitset_free(jmeno_pole) do{ free(jmeno_pole); jmeno_pole = NULL; }while(0)

    #define bitset_size(jmeno_pole) (jmeno_pole[0]>>1)

    /*
    if the past variable to this function was a negative long int, the variable index would always be smaller.
    The implementation limit is set to not use the MSB, therefore when converting negative signed long to unsigned long,
    the unsigned long will always have the MSB set to 1, so it has to be larger 
    */
    #define bitset_setbit(jmeno_pole, index, vyraz)\
        ((unsigned long)(index)<(jmeno_pole[0]>>1)?\
            ((vyraz)?\
            (jmeno_pole[1+((index)/(sizeof(unsigned long)<<3))] |= 1UL<<((index)%(sizeof(unsigned long)<<3))):\
            (jmeno_pole[1+((index)>>(4|(sizeof(unsigned long)>>2)))] &= ~(1UL<<((index)&(63>>((sizeof(unsigned long)>>2)&1)))))):\
        (((jmeno_pole[0]&1)?(free_layover = ((jmeno_pole[0]>>1)-1), free(jmeno_pole), 1):((free_layover = ((jmeno_pole[0]>>1)-1))),\
        (error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), free_layover), free_layover = 1))))
    /*
    (jmeno_pole[0]&1)?(free_layover = ((jmeno_pole[0]>>1)-1), free(jmeno_pole), 1):((free_layover = ((jmeno_pole[0]>>1)-1)))
    checking if the array was allocated dinamically or not with a mask on the LSB
    */

    /*
    if the past variable to this function was a negative long int, the variable index would always be smaller.
    The implementation limit is set to not use the MSB, therefore when converting negative signed long to unsigned long,
    the unsigned long will always have the MSB set to 1, so it has to be larger 
    */
    #define bitset_getbit(jmeno_pole, index)\
        (((unsigned long)(index)<(jmeno_pole[0]>>1))?\
        ((bool)(jmeno_pole[1+((index)>>(4|(sizeof(unsigned long)>>2)))] & (1UL<<((index)&(63>>((sizeof(unsigned long)>>2)&1)))))):\
        (((jmeno_pole[0]&1)?(free_layover = ((jmeno_pole[0]>>1)-1), free(jmeno_pole), 1):((free_layover = ((jmeno_pole[0]>>1)-1))),\
        (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)(index), free_layover), free_layover = 1))))
    /*
    (jmeno_pole[0]&1)?(free_layover = ((jmeno_pole[0]>>1)-1), free(jmeno_pole), 1):((free_layover = ((jmeno_pole[0]>>1)-1)))
    checking if the array was allocated dinamically or not with a mask on the LSB
    */

#else

    inline void bitset_free(bitset_t jmeno_pole)
    {
        free(jmeno_pole);
        jmeno_pole = NULL;
    }

    inline unsigned long bitset_size(bitset_t jmeno_pole)
    {
        return (jmeno_pole[0]>>1);
    }

    inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, bool vyraz)
    {
        /*
        if the past variable to this function was a negative long int, the variable index would always be smaller.
        The implementation limit is set to not use the MSB, therefore when converting negative signed long to unsigned long,
        the unsigned long will always have the MSB set to 1, so it has to be larger 
        */
        if (index<(jmeno_pole[0]>>1))
        {
            if (vyraz)
                jmeno_pole[1+(index>>(4|(sizeof(unsigned long)>>2)))] |= 1UL<<((index)&(63>>((sizeof(unsigned long)>>2)&1)));
            else
                jmeno_pole[1+(index>>(4|(sizeof(unsigned long)>>2)))] &= ~(1UL<<((index)&(63>>((sizeof(unsigned long)>>2)&1))));
        }
        else
        {
            if (jmeno_pole[0]&1)
            {
                free_layover = ((jmeno_pole[0]>>1) -1);
                free(jmeno_pole);
            }
            else
                free_layover = ((jmeno_pole[0]>>1) -1);
            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)index, free_layover);
        }
    }

    inline bool bitset_getbit(bitset_t jmeno_pole, bitset_index_t index)
    {
        /*
        if the past variable to this function was a negative long int, the variable index would always be smaller.
        The implementation limit is set to not use the MSB, therefore when converting negative signed long to unsigned long,
        the unsigned long will always have the MSB set to 1, so it has to be larger 
        */
        if (index<(jmeno_pole[0]>>1))
            return jmeno_pole[1+(index>>(4|(sizeof(unsigned long)>>2)))] & 1UL<<((index)&(63>>((sizeof(unsigned long)>>2)&1)));
        else
        {
            if (jmeno_pole[0]&1)
            {
                free_layover = ((jmeno_pole[0]>>1) -1);
                free(jmeno_pole);
            }
            else
                free_layover = ((jmeno_pole[0]>>1) -1);
            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)index, free_layover);
        }

        return 0;
    }

#endif //end of inline condition

#endif //end of include condition
