/* 
 * File:   cls.h
 * Author: eduardo
 *
 * Created on April 19, 2015, 8:03 PM
 */

#ifndef SLEEP_H
#define	SLEEP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//sleep
void wait(int seconds) {
	#ifdef WINDOWS
		int win_seconds = seconds * 100;
	    system("cls");
	    Sleep(win_seconds);
	#else
	    sleep(seconds);
	#endif
}

#ifdef	__cplusplus
}
#endif

#endif	/* CLS_H */
