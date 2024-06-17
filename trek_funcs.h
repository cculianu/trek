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
 */
#ifndef TREK_FUNCS_H
#define TREK_FUNCS_H

#include <stddef.h>

/* Forward declarations for global functions, to prevent modern compilers from warning. */

struct event;
struct kling;
struct quad;

extern void abandon(void);
extern void attack(int	resting /* set if attack while resting */);
extern void autover(void);
extern void capture(void);
extern void out(int dev);
extern int damaged(int dev);
extern void damage(int dev1 /*  device index */, double dam /* time to repair */);
extern struct kling *selectklingon(void);
extern char cgetc(int i);
extern int check_out(int device);
extern void checkcond(void);
extern void compkldist(int f/* set if new quadrant */);
extern void sortkl(void);
extern void skiptonl(char c);
extern int testnl(void);
extern int testterm(void);
extern void computer(void);
extern int kalc(int tqx, int tqy, int tsx, int tsy, double* dist);
extern void prkalc(int course, double dist);
extern void dcrept(void);
extern void destruct(void);
extern void dock(void);
extern void undock(void);
extern void dumpgame(void);
extern int restartgame(void);
extern int readdump(int fd1);
extern void dumpme(int flag);
extern int dumpssradio(void);
extern int events(int warp /* set if called in a time warp */);
extern int getcodi(int* co, double* di);
extern void help(void);
extern void impulse(void);
extern void initquad(int f);
extern void sector(int	*x, int *y);
extern void killk(int ix, int iy);
extern void killb(int qx, int qy);
extern void kills(int x, int y, int f);
extern void killd(int x, int y, int f);
extern void klmove(int fl);
extern void lose(int why);
extern void lrscan(void);
extern double move(int ramflag, int course, double time, double speed);
extern void nova(int x, int y);
extern void phaser(void);
extern void myreset(void);
extern void play(void);
extern void ram(int ix, int iy);

extern int ranf(int max);
extern double franf(void);
extern void rest(void);

extern struct event *schedule(int type, double offset, char x, char y, char z);
extern void reschedule(struct event *e1, double offset);
extern void unschedule(struct event *e1);
extern struct event *xsched(int ev1, int factor, int x, int y, int z);
extern void xresched(struct event *e1, int ev1, int factor);

extern long score(void);

extern void setup(void);

extern void setwarp(void);

extern void shield(int f);

extern void snova(int x, int y);

extern void srscan(int f);

extern const char *systemname(struct quad *q1);
extern const char *systemname_e(struct event *e1);

extern void torped(void);
extern int randcourse(int n);

extern void visual(void);

extern void warp(int fl, int c, double d);

extern void win(void);

extern void *bmove(const void *src, void *dest, size_t nbytes);
extern void syserr(const char *fmt, ...);
extern int sequal(const char *a, const char *b);
extern char *concat(const char *s1, const char *s2, char *s3);
extern size_t length(const char *s);

/* "Getpar" (get parameter) forward declarations: */
#include "getpar.h"

#endif // TREK_H
