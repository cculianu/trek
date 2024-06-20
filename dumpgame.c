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

#include "trek.h"

/***  THIS CONSTANT MUST CHANGE AS THE DATA SPACES CHANGE ***/
#define VERSION 3

struct dump
{
    uint16_t code;
    void    *area;
    size_t   count;
};

static const struct dump Dump_template[] = {
    {0x1234, &Ship,  sizeof(Ship) },
    {0x5678, &Now,   sizeof(Now)  },
    {0x9abc, &Param, sizeof(Param)},
    {0xdef0, &Etc,   sizeof(Etc)  },
    {0xf123, &Game,  sizeof(Game) },
    {0x4567, Sect,   sizeof(Sect) },
    {0x89ab, Quad,   sizeof(Quad) },
    {0xcdef, &Move,  sizeof(Move) },
    {0xb33f, Event,  sizeof(Event)},
    {0,      0,      0            },
};

static const char * const dumpfile = "trek.dump";

/*
**  DUMP GAME
**
**	This routine dumps the game onto the file "trek.dump".  The
**	first two bytes of the file are a version number, which
**	reflects whether this image may be used.  Obviously, it must
**	change as the size, content, or order of the data structures
**	output change.
*/

void dumpgame(void)
{
    int                version;
    FILE              *of;
    const struct dump *d;
    int                error = 0;

    if (! (of = fopen(dumpfile, "wb")))
    {
        printf("cannot open dump file for writing: %s\n", dumpfile);
        return;
    }
    /* write version int */
    version = VERSION;
    if (fwrite(&version, 1, sizeof(version), of) != sizeof(version))
    {
        error = 1;
    }

    /* output the main data areas */
    for (d = Dump_template; ! error && d->area; ++d)
    {
        error = fwrite(&d->code, 1, sizeof(d->code), of) != sizeof(d->code)
                || fwrite(&d->count, 1, sizeof(d->count), of) != sizeof(d->count);

        if (error)
            continue;

        if (d->area == &Now)
            /* temporarily modify Now to use indices for serialization */
            serialize_S_Now(d->area, (const struct S_Now *)d->area);

        error = fwrite(d->area, 1, d->count, of) != d->count;

        if (d->area == &Now)
            /* modify it back to use pointers */
            unserialize_S_Now((struct S_Now *)d->area, d->area);
    }

    if (error)
        printf("dump write failure (%s)\n", dumpfile);
    else
        printf("saved game state to dump file: %s\n", dumpfile);
    fclose(of);
}

static int readdump(FILE *fin);

/*
**  RESTORE GAME
**
**	The game is restored from the file "trek.dump".  In order for
**	this to succeed, the file must exist and be readable, must
**	have the correct version number, and must have all the appro-
**	priate data areas.
**
**	Return value is zero for success, one for failure.
*/

int restartgame(void)
{
    FILE *fin = NULL;
    int   version = ~VERSION, ret = 0;

    if ((fin = fopen(dumpfile, "rb")) == NULL || fread(&version, 1, sizeof(version), fin) != sizeof(version)
        || version != VERSION || readdump(fin) != 0)
    {
        printf("cannot restart from dumpfile: %s\n", dumpfile);
        ret = 1;
    }
    else
    {
        printf("restored game from dumpfile: %s\n", dumpfile);
        ret = 0;
    }

    if (fin)
        fclose(fin);
    return ret;
}

/*
**  READ DUMP
**
**	This is the business end of restartgame().  It reads in the
**	areas.
**
**	Returns zero for success, one for failure.
*/

static int readdump(FILE *fin)
{
    const struct dump *d;
    uint16_t           code;
    size_t             count;

    for (d = Dump_template; d->area; ++d)
    {
        if (fread(&code, 1, sizeof(code), fin) != sizeof(code) || code != d->code
            || fread(&count, 1, sizeof(count), fin) != sizeof(count) || count != d->count
            || fread(d->area, 1, count, fin) != count)
            return 1;
        if (d->area == &Now)
            /* special-case to "unserialize" this by transforming event indices back to pointers */
            unserialize_S_Now((struct S_Now *)d->area, d->area);
    }

    /* make quite certain we are at EOF */
    return feof(fin) || fread(&count, 1, 1, fin) == 0 ? 0 : 1;
}

static const size_t INVALID_EVT_IDX = ~(size_t)0u;

void serialize_S_Now(void *dest, const struct S_Now *src)
{
    size_t       i;
    struct S_Now tmp;

    memcpy(&tmp, src, sizeof(tmp));

    for (i = 0; i < (size_t)NEVENTS; ++i)
    {
        size_t idx = INVALID_EVT_IDX;
        if (tmp.u.eventptr[i])
            idx = tmp.u.eventptr[i] - &Event[0];
        if (idx >= (size_t)MAXEVENTS)
            idx = INVALID_EVT_IDX;
        tmp.u.eventidx[i] = idx;
    }

    memcpy(dest, &tmp, sizeof(tmp));
}

void unserialize_S_Now(struct S_Now *dest, const void *src)
{
    size_t       i;
    struct S_Now tmp;

    memcpy(&tmp, src, sizeof(tmp));

    for (i = 0; i < NEVENTS; ++i)
    {
        const size_t idx = tmp.u.eventidx[i];
        if (idx != INVALID_EVT_IDX && idx < MAXEVENTS)
        {
            tmp.u.eventptr[i] = &Event[idx];
        }
        else
        {
            tmp.u.eventptr[i] = 0;
        }
    }

    memcpy(dest, &tmp, sizeof(tmp));
}
