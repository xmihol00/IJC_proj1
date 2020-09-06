
//================================================================================================
// Soubor:      steg-encode.c
// Řešení:      IJC-DU1, příklad b), 4. 3. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Soubor obsahují funci main, která zajišťuje zapsání zakodované zprávy do obrázku
//                 použitím prvočísel
// Poznámky:    Opět obsahuje 2 funkce pro práci na neoptimalizované i optimalizovaném bitsetovém poli
//================================================================================================

//================================================================================================
// File:        steg-encode.c
// Case:        IJC-DU1, assignment b), 4. 3. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: File with a main function, which purpose is to encode a message into a picture
//                 using prime numbers
// Notes:       Again uses two functions first uses the non-optimized bitset array, second optimized  
//================================================================================================

#include "ppm.h"
#include "error.h"
#include "bitset.h"
#include "eratosthenes.h"

bool encode_message(bitset_t pole, p_ppm_t picture);

int main(int argc, char **argv)
{
    if (argc != 2)
        error_exit("steg-encode: Spatny pocet argumentu");

    p_ppm_t picture = ppm_read(argv[1]);
    if (picture == NULL)
        error_exit("steg-encode: Chyba nacteni souboru.");

    bitset_alloc(pole, picture->xsize*picture->ysize*3);
    Eratosthenes(pole);

    encode_message(pole, picture);

    bitset_free(pole);
    ppm_free(picture);
    return 0;
}

#ifndef OPTIMALIZATION
bool encode_message(bitset_t pole, p_ppm_t picture)
{
    if (pole == NULL)
        error_exit("encode_message: Pole není alokováno.");
    if (picture == NULL)
        error_exit("encode_message: Obrázek není alokován.");
    
    int c;
    char ch;
    unsigned long bit_count = 23;

    printf("P6\n%d %d\n255\n", picture->xsize, picture->ysize);
    while ((c = getchar()) != EOF && bit_count < bitset_size(pole))
    {
        ch = c;
        for (char i = 0; i < 8 && bit_count < bitset_size(pole); i++)
        {
            picture->data[bit_count] &= (~ 1);
            picture->data[bit_count] |= (ch & 1);
            ch >>= 1;

            while((++bit_count < bitset_size(pole)) && bitset_getbit(pole, bit_count));
        }
    }
    if (bit_count == bitset_size(pole))
    {
        for (unsigned i = 0; i < picture->xsize*picture->ysize*3; i++)
            putchar(picture->data[i]);
        warning_msg("steg-encode: Zpráva nebyla řádně ukončena.");
        return 1;
    }

    for (char i = 0; i < 8 && bit_count < bitset_size(pole); i++)
    {
        picture->data[bit_count] &= (~ 1);

        if (bit_count == bitset_size(pole) && i < 7)
        {
            for (unsigned i = 0; i < picture->xsize*picture->ysize*3; i++)
                putchar(picture->data[i]);
            warning_msg("steg-encode: Zpráva nebyla řádně ukončena.");
            return 1;
        }
        while(++bit_count < bitset_size(pole) && bitset_getbit(pole, bit_count));
    }

    for (unsigned i = 0; i < picture->xsize*picture->ysize*3; i++)
        putchar(picture->data[i]);

    return 0;
}
#else
bool encode_message(bitset_t pole, p_ppm_t picture)
{
    if (pole == NULL)
        error_exit("encode_message: Pole není alokováno.");
    if (picture == NULL)
        error_exit("encode_message: Obrázek není alokován.");
    
    int c;
    char ch;
    unsigned long bit_count = 23 >> 1;

    printf("P6\n%d %d\n255\n", picture->xsize, picture->ysize);
    while ((c = getchar()) != EOF && bit_count < bitset_size(pole))
    {
        ch = c;
        for (char i = 0; i < 8 && bit_count < bitset_size(pole); i++)
        {
            picture->data[(bit_count<<1)+1] &= (~ 1);
            picture->data[(bit_count<<1)+1] |= (ch & 1);
            ch >>= 1;

            while((++bit_count < bitset_size(pole)) && bitset_getbit(pole, bit_count));

        }
    }
    if (bit_count == bitset_size(pole))
    {
        for (unsigned i = 0; i < picture->xsize*picture->ysize*3; i++)
            putchar(picture->data[i]);
        warning_msg("steg-encode: Zpráva nebyla řádně ukončena.");
        return 1;
    }

    for (char i = 0; i < 8 && bit_count < bitset_size(pole); i++)
    {
        picture->data[(bit_count<<1)+1] &= ((~ 0) - 1);

        if (bit_count == bitset_size(pole) && i < 7)
        {
            for (unsigned i = 0; i < picture->xsize*picture->ysize*3; i++)
                putchar(picture->data[i]);
            warning_msg("steg-encode: Zpráva nebyla řádně ukončena.");
            return 1;
        }
        while(++bit_count < bitset_size(pole) && bitset_getbit(pole, bit_count));
    }

    for (unsigned i = 0; i < picture->xsize*picture->ysize*3; i++)
        putchar(picture->data[i]);

    return 0;
}
#endif