/* 
 * File:   cls.h
 * Author: eduardo
 *
 * Created on April 19, 2015, 8:03 PM
 */

#ifndef SLEEP_H
#define	SLEEP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//sleep
void wait(int seconds) {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		int win_seconds = seconds * 1000;
	    sleep(win_seconds);
	#else
	    sleep(seconds);
	#endif
}

#ifdef	__cplusplus
}
#endif

#endif	/* CLS_H */
