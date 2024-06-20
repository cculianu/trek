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
 *
 *	@(#)trek.h	5.5 (Berkeley) 6/1/90
 */

/*
**  Global Declarations
**
**	Virtually all non-local variable declarations are made in this
**	file.  Exceptions are those things which are initialized, which
**	are defined in "externs.c", and things which are local to one
**	program file.
**
**	So far as I know, nothing in here must be preinitialized to
**	zero.
**
**	You may have problems from the loader if you move this to a
**	different machine.  These things actually get allocated in each
**	source file, which UNIX allows; however, you may (on other
**	systems) have to change everything in here to be "extern" and
**	actually allocate stuff in "externs.c"
**
**      ** NOTE from Calin **
**        Yes, I moved everything to externs.c since on some systems
**        the above was a problem.
*/
#ifndef TREK_H
#define TREK_H

/* These headers, while not accessed here, are used in various places in the program and are included here. */
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for memcpy(), etc */

/*********************  GALAXY  **************************/

/* galactic parameters */
#define NSECTS 10 /* dimensions of quadrant in sectors */
#define NQUADS 8  /* dimension of galazy in quadrants */
#define NINHAB 32 /* number of quadrants which are inhabited */

typedef int8_t i8;

struct quad /* definition for each quadrant */
{
    i8  bases;       /* number of bases in this quadrant */
    i8  klings;      /* number of Klingons in this quadrant */
    i8  holes;       /* number of black holes in this quadrant */
    int scanned;     /* star chart entry (see below) */
    i8  stars;       /* number of stars in this quadrant */
    i8  qsystemname; /* starsystem name (see below) */
};

#define Q_DISTRESSED 0200
#define Q_SYSTEM     077

/*  systemname conventions:
 *	1 -> NINHAB	index into Systemname table for live system.
 *	+ Q_DISTRESSED	distressed starsystem -- systemname & Q_SYSTEM
 *			is the index into the Event table which will
 *			have the system name
 *	0		dead or nonexistent starsystem
 *
 *  starchart ("scanned") conventions:
 *	0 -> 999	taken as is
 *	-1		not yet scanned ("...")
 *	1000		supernova ("///")
 *	1001		starbase + ??? (".1.")
 */

/* ascii names of systems */
extern const char * const Systemname[NINHAB];

/* quadrant definition */
extern struct quad Quad[NQUADS][NQUADS];

/* defines for sector map  (below) */
#define EMPTY      '.'
#define STAR       '*'
#define BASE       '#'
#define ENTERPRISE 'E'
#define QUEENE     'Q'
#define KLINGON    'K'
#define INHABIT    '@'
#define HOLE       ' '

/* current sector map */
extern char Sect[NSECTS][NSECTS];

/************************ DEVICES ******************************/

#define NDEV 16 /* max number of devices */

/* device tokens */
#define WARP     0  /* warp engines */
#define SRSCAN   1  /* short range scanners */
#define LRSCAN   2  /* long range scanners */
#define PHASER   3  /* phaser control */
#define TORPED   4  /* photon torpedo control */
#define IMPULSE  5  /* impulse engines */
#define SHIELD   6  /* shield control */
#define COMPUTER 7  /* on board computer */
#define SSRADIO  8  /* subspace radio */
#define LIFESUP  9  /* life support systems */
#define SINS     10 /* Space Inertial Navigation System */
#define CLOAK    11 /* cloaking device */
#define XPORTER  12 /* transporter */
#define SHUTTLE  13 /* shuttlecraft */

/* device names */
struct device
{
    const char *name;   /* device name */
    const char *person; /* the person who fixes it */
};

extern const struct device Device[NDEV];

/***************************  EVENTS  ****************************/

#define NEVENTS 12 /* number of different event types */

#define E_LRTB   1  /* long range tractor beam */
#define E_KATSB  2  /* Klingon attacks starbase */
#define E_KDESB  3  /* Klingon destroys starbase */
#define E_ISSUE  4  /* distress call is issued */
#define E_ENSLV  5  /* Klingons enslave a quadrant */
#define E_REPRO  6  /* a Klingon is reproduced */
#define E_FIXDV  7  /* fix a device */
#define E_ATTACK 8  /* Klingon attack during rest period */
#define E_SNAP   9  /* take a snapshot for time warp */
#define E_SNOVA  10 /* supernova occurs */

#define E_GHOST  0100 /* ghost of a distress call if ssradio out */
#define E_HIDDEN 0200 /* event that is unreportable because ssradio out */
#define E_EVENT  077  /* mask to get event code */

struct event
{
    i8     x, y;       /* coordinates */
    double date;       /* trap stardate */
    i8     evcode;     /* event type */
    i8     systemname; /* starsystem name */
};
/* systemname conventions:
 *	1 -> NINHAB	index into Systemname table for reported distress calls
 *
 * evcode conventions:
 *	1 -> NEVENTS-1	event type
 *	+ E_HIDDEN	unreported (SSradio out)
 *	+ E_GHOST	actually already expired
 *	0		unallocated
 */

#define MAXEVENTS 25 /* max number of concurrently pending events */

extern struct event Event[MAXEVENTS]; /* dynamic event list; one entry per pending event */

/*****************************  KLINGONS  *******************************/

struct kling
{
    i8     x, y;    /* coordinates */
    int    power;   /* power left */
    double dist;    /* distance to Enterprise */
    double avgdist; /* average over this move */
    i8     srndreq; /* set if surrender has been requested */
};

#define MAXKLQUAD 9 /* maximum klingons per quadrant */

/********************** MISCELLANEOUS ***************************/

/* condition codes */
#define GREEN  0
#define DOCKED 1
#define YELLOW 2
#define RED    3

/* starbase coordinates */
#define MAXBASES 9 /* maximum number of starbases in galaxy */

/*  distress calls  */
#define MAXDISTR 5 /* maximum concurrent distress calls */

/* phaser banks */
#define NBANKS 6 /* number of phaser banks */

struct xy
{
    i8 x, y; /* coordinates */
};

/*
 *	note that much of the stuff in the following structs CAN NOT
 *	be moved around!!!!
 */

/* information regarding the state of the starship */
extern struct S_Ship
{
    double warp;         /* warp factor */
    double warp2;        /* warp factor squared */
    double warp3;        /* warp factor cubed */
    i8     shldup;       /* shield up flag */
    i8     cloaked;      /* set if cloaking device on */
    int    energy;       /* starship's energy */
    int    shield;       /* energy in shields */
    double reserves;     /* life support reserves */
    int    crew;         /* ship's complement */
    int    brigfree;     /* space left in brig */
    i8     torped;       /* torpedoes */
    i8     cloakgood;    /* set if we have moved */
    int    quadx;        /* quadrant x coord */
    int    quady;        /* quadrant y coord */
    int    sectx;        /* sector x coord */
    int    secty;        /* sector y coord */
    i8     cond;         /* condition code */
    i8     sinsbad;      /* Space Inertial Navigation System condition */
    char   shipname[16]; /* name of current starship */
    i8     ship;         /* current starship */
    int    distressed;   /* number of distress calls */
} Ship;

/* sinsbad is set if SINS is working but not calibrated */

/* game related information, mostly scoring */
extern struct S_Game
{
    int  killk;      /* number of klingons killed */
    int  deaths;     /* number of deaths onboard Enterprise */
    i8   negenbar;   /* number of hits on negative energy barrier */
    i8   killb;      /* number of starbases killed */
    int  kills;      /* number of stars killed */
    i8   skill;      /* skill rating of player */
    i8   length;     /* length of game */
    i8   killed;     /* set if you were killed */
    i8   killinhab;  /* number of inhabited starsystems killed */
    i8   tourn;      /* set if a tournament game */
    char passwd[15]; /* game password */
    i8   snap;       /* set if snapshot taken */
    i8   helps;      /* number of help calls */
    int  captives;   /* total number of captives taken */
} Game;

/* per move information */
extern struct S_Move
{
    i8     free;    /* set if a move is free */
    i8     endgame; /* end of game flag */
    i8     shldchg; /* set if shields changed this move */
    i8     newquad; /* set if just entered this quadrant */
    i8     resting; /* set if this move is a rest */
    double time;    /* time used this move */
} Move;

/* parametric information */
extern struct S_Param
{
    i8     bases;             /* number of starbases */
    i8     klings;            /* number of klingons */
    double date;              /* stardate */
    double time;              /* time left */
    double resource;          /* Federation resources */
    int    energy;            /* starship's energy */
    int    shield;            /* energy in shields */
    double reserves;          /* life support reserves */
    int    crew;              /* size of ship's complement */
    int    brigfree;          /* max possible number of captives */
    i8     torped;            /* photon torpedos */
    double damfac[NDEV];      /* damage factor */
    double dockfac;           /* docked repair time factor */
    double regenfac;          /* regeneration factor */
    int    stopengy;          /* energy to do emergency stop */
    int    shupengy;          /* energy to put up shields */
    int    klingpwr;          /* Klingon initial power */
    int    warptime;          /* time chewer multiplier */
    double phasfac;           /* Klingon phaser power eater factor */
    i8     moveprob[6];       /* probability that a Klingon moves */
    double movefac[6];        /* Klingon move distance multiplier */
    double eventdly[NEVENTS]; /* event time multipliers */
    double navigcrud[2];      /* navigation crudup factor */
    int    cloakenergy;       /* cloaking device energy per stardate */
    double damprob[NDEV];     /* damage probability */
    double hitfac;            /* Klingon attack factor */
    int    klingcrew;         /* number of Klingons in a crew */
    double srndrprob;         /* surrender probability */
    int    energylow;         /* low energy mark (cond YELLOW) */
} Param;

/* Sum of damage probabilities must add to 1000 */

/* other information kept in a snapshot */
extern struct S_Now
{
    i8     bases;      /* number of starbases */
    i8     klings;     /* number of klingons */
    double date;       /* stardate */
    double time;       /* time left */
    double resource;   /* Federation resources */
    i8     distressed; /* number of currently distressed quadrants */
    union
    {
        struct event *eventptr[NEVENTS]; /* pointer to event structs */
        size_t        eventidx[NEVENTS]; /* Offset from Event[0] base (serialization only) */
    } u;
    struct xy base[MAXBASES]; /* locations of starbases */
} Now;

void serialize_S_Now(void *dest, const struct S_Now *src);
void unserialize_S_Now(struct S_Now *dest, const void *src);

/* Other stuff, not dumped in a snapshot */
extern struct S_Etc
{
    struct kling klingon[MAXKLQUAD];                                /* sorted Klingon list */
    i8           nkling;                                            /* number of Klingons in this sector */
                                                                    /* < 0 means automatic override mode */
    i8           fast;                                              /* set if speed > 300 baud (unused) */
    struct xy    starbase;                                          /* starbase in current quadrant */
    char         snapshot[sizeof Quad + sizeof Event + sizeof Now]; /* snapshot for time warp */
    i8           statreport;                                        /* set to get a status report on a srscan */
} Etc;

/*
 *	eventptr is a pointer to the event[] entry of the last
 *	scheduled event of each type.  Zero if no such event scheduled.
 */

/* Klingon move indicies */
#define KM_OB 0 /* Old quadrant, Before attack */
#define KM_OA 1 /* Old quadrant, After attack */
#define KM_EB 2 /* Enter quadrant, Before attack */
#define KM_EA 3 /* Enter quadrant, After attack */
#define KM_LB 4 /* Leave quadrant, Before attack */
#define KM_LA 5 /* Leave quadrant, After attack */

/* you lose codes */
#define L_NOTIME   1  /* ran out of time */
#define L_NOENGY   2  /* ran out of energy */
#define L_DSTRYD   3  /* destroyed by a Klingon */
#define L_NEGENB   4  /* ran into the negative energy barrier */
#define L_SUICID   5  /* destroyed in a nova */
#define L_SNOVA    6  /* destroyed in a supernova */
#define L_NOLIFE   7  /* life support died (so did you) */
#define L_NOHELP   8  /* you could not be rematerialized */
#define L_TOOFAST  9  /* pretty stupid going at warp 10 */
#define L_STAR     10 /* ran into a star */
#define L_DSTRCT   11 /* self destructed */
#define L_CAPTURED 12 /* captured by Klingons */
#define L_NOCREW   13 /* you ran out of crew */

/******************  COMPILE OPTIONS  ***********************/

/* Trace info */
#define xTRACE 1
extern int Trace;

/***************  FORWARD DECLARATIONS  ********************/

/* Forward declarations for global functions, to prevent
 * modern compilers from warning. */

extern void          abandon(void);
extern void          attack(int resting);
extern void          autover(void);
extern void          capture(void);
extern void          out(int dev);
extern int           damaged(int dev);
extern void          damage(int dev, double dam);
extern struct kling *selectklingon(void);
extern char          cgetc(void);
extern int           check_out(int dev);
extern void          checkcond(void);
extern void          compkldist(int f);
extern void          sortkl(void);
extern void          skiptonl(char c);
extern int           testnl(void);
extern int           testterm(void);
extern void          computer(void);
extern int           kalc(int tqx, int tqy, int tsx, int tsy, double *dist);
extern void          prkalc(int course, double dist);
extern void          dcrept(void);
extern void          destruct(void);
extern void          dock(void);
extern void          undock(void);
extern void          dumpgame(void);
extern int           restartgame(void);
extern void          dumpme(int flag);
extern int           dumpssradio(void);
extern int           events(int warp);
extern int           getcodi(int *co, double *di);
extern void          help(void);
extern void          impulse(void);
extern void          initquad(int f);
extern void          sector(int *x, int *y);
extern void          killk(int ix, int iy);
extern void          killb(int qx, int qy);
extern void          kills(int x, int y, int f);
extern void          killd(int x, int y, int f);
extern void          klmove(int fl);
extern void          lose(int why);
extern void          lrscan(void);
extern double        move(int ramflag, int course, double time, double speed);
extern void          nova(int x, int y);
extern void          phaser(void);
extern void          myreset(void);
extern void          play(void);
extern void          ram(int ix, int iy);
extern int           ranf(int max);
extern double        franf(void);
extern void          rest(void);
extern struct event *schedule(int type, double offset, int x, int y, int z);
extern void          reschedule(struct event *e, double offset);
extern void          unschedule(struct event *e);
extern struct event *xsched(int ev, int factor, int x, int y, int z);
extern void          xresched(struct event *e, int ev, int factor);
extern long          score(void);
extern void          setup(void);
extern void          setwarp(void);
extern void          shield(int f);
extern void          snova(int x, int y);
extern void          srscan(int f);
extern const char   *systemname(struct quad *q);
extern const char   *systemname_e(struct event *e);
extern void          torped(void);
extern int           randcourse(int n);
extern void          visual(void);
extern void          warp(int fl, int c, double d);
extern void          win(void);
/* utility.c: */
extern void         *bmove(const void *src, void *dest, size_t nbytes);
extern char         *concat(const char *s1, const char *s2, char *s3);
extern size_t        length(const char *s);                /* equivalent to strlen() */
extern int           sequal(const char *a, const char *b); /* misnamed; true if a & b are prefixes of each other */
extern void          sleep_secs(unsigned seconds);         /* equivalent to unix sleep() */
extern void          syserr(const char *fmt, ...);         /* prints error then exits */

/* "Getpar" (get parameter) forward declarations: */
#include "getpar.h"

#endif /* TREK_H */
