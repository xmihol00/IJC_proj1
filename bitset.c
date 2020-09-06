//================================================================================================
// Soubor:      bitset.c
// Řešení:      IJC-DU1, příklad a), 21. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       definice inline funkcí 
//================================================================================================


#include "bitset.h"

// has to be compiled also with the version without inline functions
unsigned long free_layover;

#ifdef USE_INLINE
extern inline void bitset_free(bitset_t jmeno_pole);

extern unsigned long bitset_size(bitset_t jmeno_pole);

extern void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, _Bool vyraz);

extern bool bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);
#endif