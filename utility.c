/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static char sccsid[] = "@(#)utility.c	5.4 (Berkeley) 6/1/90";
#endif /* not lint */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

/*
**  ASSORTED UTILITY ROUTINES
*/

/*
**  BLOCK MOVE
**
**	Moves a block of storage of length `l' bytes from the data
**	area pointed to by `a' to the area pointed to by `b'.
**	Returns the address of the byte following the `b' field.
**	Overflow of `b' is not tested.
*/

void *bmove(const void *a1, void *b1, size_t l)
{
    const char *a = a1;
    char *b = b1;
    while (l--)
        *b++ = *a++;
    return b;
}


/*
**  STRING EQUALITY TEST
**	null-terminated strings `a' and `b' are tested for
**	absolute equality.
**	returns one if equal, zero otherwise.
*/

int sequal(const char *a, const char *b)
{
        register const char	*p, *q;

	p = a;
	q = b;
	while (*p || *q)
		if (*p++ != *q++)
			return(0);
	return(1);
}


/*
**  STRING CONCATENATE
**
**	The strings `s1' and `s2' are concatenated and stored into
**	`s3'.  It is ok for `s1' to equal `s3', but terrible things
**	will happen if `s2' equals `s3'.  The return value is is a
**	pointer to the end of `s3' field.
*/

char *concat(const char *s1, const char *s2, char *s3)
{
        register       char	*p;
        register const char	*q;

	p = s3;
	q = s1;
	while (*q)
		*p++ = *q++;
	q = s2;
	while (*q)
		*p++ = *q++;
	*p = 0;
	return (p);
}


void syserr(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf("\n\07TREK SYSERR: ");
    vprintf(fmt, ap);
    printf("\n");
    if (errno)
        printf("\tsystem error %d: %s\n", errno, strerror(errno));
    va_end(ap);
    exit(EXIT_FAILURE);
}

/*
**  FIND STRING LENGTH
**
**	The length of string `s' (excluding the null byte which
**		terminates the string) is returned.
*/

size_t length(const char *s) { return strlen(s); }

void sleep_secs(unsigned seconds)
{
#if defined(HAVE_UNISTD_H)
    sleep(seconds);
#elif defined(_WIN32)
    Sleep(seconds * 1000u);
#else
#error "Unknown platform!"
#endif
}
