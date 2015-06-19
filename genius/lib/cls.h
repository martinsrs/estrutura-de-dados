/* 
 * File:   cls.h
 * Author: eduardo
 *
 * Created on April 19, 2015, 8:03 PM
 */

#ifndef CLS_H
#define	CLS_H

#ifdef __cplusplus__
  #include <cstdlib>
#else
  #include <stdlib.h>
#endif


#include <stdio.h>
#include <stdlib.h>


/**
 * Limpa a tela, fazendo chamadas de sistema win ou linux
 */
void clear_screen() {
	if (system("CLS")) system("clear");
}


#ifdef	__cplusplus
}
#endif

#endif	/* CLS_H */
