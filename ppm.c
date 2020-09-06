//================================================================================================
// Soubor:      ppm.c
// Řešení:      IJC-DU1, příklad b), 21. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Definice fuknce ppm_read a ppm_free. 
//================================================================================================

//================================================================================================
// File:        ppm.c
// Case:        IJC-DU1, assignment b), 21. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: Definition of functions ppm_read and ppm_free. 
//================================================================================================

#include "ppm.h"

struct ppm *ppm_read(const char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        warning_msg("ppm_read: Otevření souboru selhalo");
        return NULL;
    }
    
    unsigned width;
    unsigned height;
    unsigned color;
    if (fscanf(fptr, "P6%u%u%u", &width, &height, &color) != 3 || color != 255)
    {
        warning_msg("ppm_read: Špatný formát souboru.");
        goto close_file;       
    }
    
    if (height <= 0 || width <= 0)
    {
        warning_msg("ppm_read: Šířka ani výška obrázku nemohou být záporné nebo nulové.");
        goto close_file;
    }
    //variable width can't be 0 at this point
    if (PICTURE_MAX_SIZE/width <= height)
    {
        warning_msg("ppm_read: Velikost obrázku přesahuje implementačni limit.");
        goto close_file;     
    }

    p_ppm_t tmp = malloc(2*sizeof(unsigned) + width*height*3);
    if (tmp == NULL)
    {
        warning_msg("ppm_read: Chyba alokace paměti.");
        goto close_file;
    }

    tmp->xsize = width;
    tmp->ysize = height;

    getc(fptr); // to clear the whitespace after the color specification

    if (fread(tmp->data, width*height*3, 1, fptr) != 1)
    {
        warning_msg("ppm_read: Soubor obsahuje méně pixelů, než je specifikováno.");
        goto free_memory_and_close_file;
    }

    if (getc(fptr) != EOF)
    {
        warning_msg("ppm_read: Soubor obsahuje více pixelů, než je specifikováno.");
        goto free_memory_and_close_file;
    }

    fclose(fptr);
    return tmp;


    //when error occures
    free_memory_and_close_file:
        ppm_free(tmp);
    
    close_file:
        fclose(fptr);

    return NULL;
}

void ppm_free(struct ppm *p)
{
    free(p);
    p = NULL;
}