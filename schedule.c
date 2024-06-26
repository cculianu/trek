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

/*
**  SCHEDULE AN EVENT
**
**	An event of type 'type' is scheduled for time NOW + 'offset'
**	into the first available slot.  'x', 'y', and 'z' are
**	considered the attributes for this event.
**
**	The address of the slot is returned.
*/

struct event *schedule(int type, double offset, int x, int y, int z)
{
    struct event *e;
    int           i;
    double        date;

    date = Now.date + offset;
    for (i = 0; i < MAXEVENTS; i++)
    {
        e = &Event[i];
        if (e->evcode)
            continue;
            /* got a slot */
#ifdef xTRACE
        if (Trace)
            printf("schedule: type %d @ %.2f slot %d parm %d %d %d\n", type, date, i, x, y, z);
#endif
        e->evcode = type;
        e->date = date;
        e->x = x;
        e->y = y;
        e->systemname = z;
        Now.u.eventptr[type] = e;
        return e;
    }
    syserr("Cannot schedule event %d parm %d %d %d", type, x, y, z);
    return NULL;
}

/*
**  RESCHEDULE AN EVENT
**
**	The event pointed to by 'e' is rescheduled to the current
**	time plus 'offset'.
*/

void reschedule(struct event *e, double offset)
{
    double date;

    date = Now.date + offset;
    e->date = date;
#ifdef xTRACE
    if (Trace)
        printf("reschedule: type %d parm %d %d %d @ %.2f\n", e->evcode, (int)e->x, (int)e->y, (int)e->systemname, date);
#endif
}

/*
**  UNSCHEDULE AN EVENT
**
**	The event at slot 'e' is deleted.
*/

void unschedule(struct event *e)
{
#ifdef xTRACE
    if (Trace)
        printf("unschedule: type %d @ %.2f parm %d %d %d\n", e->evcode, e->date, (int)e->x, (int)e->y,
               (int)e->systemname);
#endif
    Now.u.eventptr[e->evcode & E_EVENT] = 0;
    e->date = 1e50;
    e->evcode = 0;
    return;
}

/*
**  Abreviated schedule routine
**
**	Parameters are the event index and a factor for the time
**	figure.
*/

struct event *xsched(int ev, int factor, int x, int y, int z)
{
    return schedule(ev, -Param.eventdly[ev] * Param.time * log(franf()) / factor, x, y, z);
}

/*
**  Simplified reschedule routine
**
**	Parameters are the event index, the initial date, and the
**	division factor.  Look at the code to see what really happens.
*/

void xresched(struct event *e, int ev, int factor)
{
    reschedule(e, -Param.eventdly[ev] * Param.time * log(franf()) / factor);
}
