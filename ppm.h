
//================================================================================================
// Soubor:      ppm.h
// Řešení:      IJC-DU1, příklad b), 21. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Deklarace struktury ppm a na ní operujících funkcí 
//================================================================================================

//================================================================================================
// File:        ppm.h
// Case:        IJC-DU1, assignment b), 21. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: Declaration of functions ppm_read and ppm_free. 
//================================================================================================

#ifndef _PPM_H
#define _PPM_H

#include <stdlib.h>
#include <stdio.h>
#include "error.h"

#define PICTURE_MAX_SIZE 200000001L

struct ppm 
{
    unsigned xsize;
    unsigned ysize;
    char data[];    // RGB bajty, celkem 3*xsize*ysize
};

typedef struct ppm ppm_t;
typedef struct ppm* p_ppm_t;

struct ppm *ppm_read(const char *filename);

void ppm_free(struct ppm *p);

#else
#endif