//================================================================================================
// Soubor:      steg-decode.c
// Řešení:      IJC-DU1, příklad b), 28. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Soubor obsahují funci main, která zajišťuje vypsání zakodované zprávy v obrázku
//                 použitím bitsetového pole
// Poznámky:    Opět obsahuje 2 funkce pro práci na neoptimalizované i optimalizovaném bitsetovém poli
//================================================================================================

//================================================================================================
// File:        steg-decode.c
// Case:        IJC-DU1, assignment b), 28. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: File with a main function, which purpose is to decode a message encoded in a picture
//                 using prime numbers
// Notes:       Again uses two functions first uses the non-optimized bitset array, second optimized  
//================================================================================================

#include "ppm.h"
#include "error.h"
#include "bitset.h"
#include "eratosthenes.h"

int decode_message(bitset_t pole, p_ppm_t picture);

int main(int argc, char *argv[])
{
    if (argc != 2)
        error_exit("steg-decode: Spatny pocet argumentu");

    p_ppm_t picture = ppm_read(argv[1]);
    if (picture == NULL)
        error_exit("steg-decode: Chyba nacteni souboru.");

    bitset_alloc(pole, picture->xsize*picture->ysize*3);

    Eratosthenes(pole);
    if (decode_message(pole, picture))
    {
        bitset_free(pole);
        ppm_free(picture);
        return 1;
    }

    bitset_free(pole);
    ppm_free(picture);
    return 0;
}

#ifndef OPTIMALIZATION
int decode_message(bitset_t pole, p_ppm_t picture)
{
    if (pole == NULL && picture != NULL)
    {
        ppm_free(picture);
        error_exit("message: Pole není alokováno.");
    }
    
    if (picture == NULL && pole != NULL)
    {
        bitset_free(pole);
        error_exit("message: Obrázek není alokován.");
    }

    if (pole == NULL && picture == NULL)
        error_exit("message: Obrázek ani pole nejsou alokovány.");
    
    char ch = 0;

    unsigned char bit_count = 0;
    for (unsigned long i = 23; i<bitset_size(pole);)
    {
        while(bitset_getbit(pole, i))
        {
            i++;
            if (i >= bitset_size(pole))
                break;
        }

        ch |= (picture->data[i++]&1)<<(bit_count++);

        if (bit_count == CHAR_BIT)
        {
            bit_count = 0;
            if (ch == '\0')
            {
                putchar('\n');
                return 0;
            }
            
            putchar(ch);
            ch = 0;
        }

    }

    warning_msg("message: Zprava nebyla spravne ukoncena.");
    return 1;
}
#else
int decode_message(bitset_t pole, p_ppm_t picture)
{
    if (pole == NULL && picture != NULL)
    {
        ppm_free(picture);
        error_exit("message: Pole není alokováno.");
    }
    
    if (picture == NULL && pole != NULL)
    {
        bitset_free(pole);
        error_exit("message: Obrázek není alokován.");
    }

    if (pole == NULL && picture == NULL)
        error_exit("message: Obrázek ani pole nejsou alokovány.");
    
    char ch = 0;

    unsigned char bit_count = 0;
    for (unsigned long i = 23>>1; i<bitset_size(pole);)
    {
        while(bitset_getbit(pole, i))
        {
            i++;
            if (i == bitset_size(pole))
                break;
        }

        ch |= (picture->data[(i<<1)+1]&1)<<(bit_count++);
        i++;

        if (bit_count == CHAR_BIT)
        {
            bit_count = 0;
            if (ch == '\0')
            {
                putchar('\n');
                return 0;
            }
            
            putchar(ch);
            ch = 0;
        }

    }

    warning_msg("message: Zprava nebyla spravne ukoncena.");
    return 1;
}
#endif
