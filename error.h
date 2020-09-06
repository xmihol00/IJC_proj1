//================================================================================================
// Soubor:      error.h
// Řešení:      IJC-DU1, příklad b), 25. 2. 2020
// Autor:       David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Přeloženo:   gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Popis:       Deklarace fukncí, které řeší chybové stavy
//================================================================================================

//================================================================================================
// File:        error.h
// Case:        IJC-DU1, assignment b), 25. 2. 2020
// Author:      David Mihola, FIT, xmihol00@stud.fit.vutbr.cz
// Compiled:    gcc (Ubuntu 9.2.1-9ubuntu2) 9.2.1 20191008
// Description: Declaration of functions for error handling 
//================================================================================================

#ifndef _ERROR_H
#define _ERROR_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void warning_msg(const char *fmt, ...);
void error_exit(const char *fmt, ...);

#endif 