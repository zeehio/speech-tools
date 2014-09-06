 /************************************************************************/
 /*                                                                      */
 /*                Centre for Speech Technology Research                 */
 /*                     University of Edinburgh, UK                      */
 /*                       Copyright (c) 1996,1997                        */
 /*                        All Rights Reserved.                          */
 /*                                                                      */
 /*  Permission is hereby granted, free of charge, to use and distribute */
 /*  this software and its documentation without restriction, including  */
 /*  without limitation the rights to use, copy, modify, merge, publish, */
 /*  distribute, sublicense, and/or sell copies of this work, and to     */
 /*  permit persons to whom this work is furnished to do so, subject to  */
 /*  the following conditions:                                           */
 /*   1. The code must retain the above copyright notice, this list of   */
 /*      conditions and the following disclaimer.                        */
 /*   2. Any modifications must be clearly marked as such.               */
 /*   3. Original authors' names are not deleted.                        */
 /*   4. The authors' names are not used to endorse or promote products  */
 /*      derived from this software without specific prior written       */
 /*      permission.                                                     */
 /*                                                                      */
 /*  THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK       */
 /*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING     */
 /*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT  */
 /*  SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE    */
 /*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   */
 /*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN  */
 /*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,         */
 /*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF      */
 /*  THIS SOFTWARE.                                                      */
 /*                                                                      */
 /************************************************************************/
 /*                 Author: Richard Caley (rjc@cstr.ed.ac.uk)            */
 /*                   Date: Wed Apr  9 1997                              */
 /************************************************************************/
 /*                                                                      */
 /* Allocation routines which check for errors.                          */
 /*                                                                      */
 /************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include "EST_unix.h"
#include <string.h>
#include "EST_cutils.h"

void *safe_walloc(size_t size)
{
    void *p;
    
    if (size == 0)
	/* Some mallocs return NULL for size 0, which means you can't tell
	   if it failed or not. So we'll avoid that problem by never 
	   asking for 0 bytes */
	p = malloc(1);
    else
	p = malloc(size);

    if (p == NULL)
    {
	fprintf(stderr,"WALLOC: failed to malloc %zu bytes\n",size);
	exit(-1);  /* I'd rather not do this but this is the only safe */
	           /* thing to do */
    }

    return p;
}

void *safe_wrealloc(void *ptr, size_t size)
{
    void *p;

    if (ptr == 0)
	p = safe_walloc(size);
    else if (size == 0)
	/* Some mallocs return NULL for size 0, which means you can't tell
	   if it failed or not. So we'll avoid that problem by never 
	   asking for 0 bytes */
	p = realloc(ptr, 1);
    else
	p = realloc(ptr, size);

    if ((p == NULL) && (size != 0))
    {
	fprintf(stderr,"WREALLOC: failed to malloc %zu bytes\n",size);
	exit(-1);  /* I'd rather not do this but this is the only safe */
	           /* thing to do */
    }

    return p;
}

void *safe_wcalloc(size_t num, size_t size)
{
    void *p;
    /* Some mallocs return NULL for size 0, which means you can't tell
	   if it failed or not. So we'll avoid that problem by never 
	   asking for 0 bytes */
    if (num == 0 || size == 0) {
		p = calloc(1, 1);
	} else {
		p = calloc(num, size);
	}

    if (p == NULL)
    {
	fprintf(stderr,"WCALLOC: failed to calloc %zu elements of %zu bytes each\n",num, size);
	exit(-1);  /* I'd rather not do this but this is the only safe */
	           /* thing to do */
    }
    return p;
}

char *wstrdup(const char *s)
{
    size_t strsize=strlen(s);
    if (strsize < SIZE_MAX) {
      strsize++; /* We can finish copy with a zero */
    } else {
      fprintf(stderr,"String is not null terminated! (too long)\n");
      exit(-1);
    }
    char *t = safe_wcalloc(sizeof(char), strsize);
    memcpy(t, s, strsize);
    return t;
}

void wfree(void *p)
{
    if (p != NULL)
	free(p);
}

