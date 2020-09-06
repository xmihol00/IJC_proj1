//================================================================================================
// Soubor:      error.c
// Řešení:      IJC-DU1, příklad b), 21. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Definice fukncí, které řeší chybové stavy. 
//================================================================================================

//================================================================================================
// File:        error.c
// Case:        IJC-DU1, assignment b), 25. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: Definition of functions for error handling 
//================================================================================================

#include "error.h"

void error_exit(const char *fmt, ...)
{
    va_list arguments;

    fprintf(stderr, "CHYBA: ");

    va_start(arguments, fmt);
    vfprintf(stderr, fmt, arguments);

    fprintf(stderr, "\n");
    va_end(arguments);
    
    exit(1);
}

void warning_msg(const char *fmt, ...)
{
    va_list arguments;

    fprintf(stderr, "CHYBA: ");

    va_start(arguments, fmt);
    vfprintf(stderr, fmt, arguments);

    fprintf(stderr, "\n");

    va_end(arguments);   
}