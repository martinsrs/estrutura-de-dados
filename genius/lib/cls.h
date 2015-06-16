/* 
 * File:   cls.h
 * Author: eduardo
 *
 * Created on April 19, 2015, 8:03 PM
 */

#ifndef CLS_H
#define	CLS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

void cls();

// limpa a tela
void clear_screen() {
	#ifdef WINDOWS
	    system("cls");
	#else
	    // Assume POSIX
	    system ("clear");
	#endif
}


#ifdef	__cplusplus
}
#endif

#endif	/* CLS_H */