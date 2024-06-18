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

#include <setjmp.h>

#include "trek.h"

/*
**  INSTRUCTION READ AND MAIN PLAY LOOP
**
**	Well folks, this is it.  Here we have the guts of the game.
**	This routine executes moves.  It sets up per-move variables,
**	gets the command, and executes the command.  After the command,
**	it calls events() to use up time, attack() to have Klingons
**	attack if the move was not free, and checkcond() to check up
**	on how we are doing after the move.
*/

/* clang-format off */
static void my_abandon(int i) { (void)i; abandon(); }
static void my_capture(int i) { (void)i; capture(); }
static void my_computer(int i) { (void)i; computer(); }
static void my_dcrept(int i) { (void)i; dcrept(); }
static void my_destruct(int i) { (void)i; destruct(); }
static void my_dock(int i) { (void)i; dock(); }
static void my_help(int i) { (void)i; help(); }
static void my_impulse(int i) { (void)i; impulse(); }
static void my_lrscan(int i) { (void)i; lrscan(); }
static void my_warp(int fl) { warp(fl, 0, 0); }
static void my_phaser(int i) { (void)i; phaser(); }
static void my_dumpgame(int i) { (void)i; dumpgame(); }
static void my_rest(int i) { (void)i; rest(); }
static void my_myreset(int i) { (void)i; myreset(); }
static void my_torped(int i) { (void)i; torped(); }
static void my_undock(int i) { (void)i; undock(); }
static void my_visual(int i) { (void)i; visual(); }
static void my_setwarp(int i) { (void)i; setwarp(); }
/* clang-format on */

static struct cvntab Comtab[] = {
    {"abandon",   "",        {my_abandon},  0 },
    {"ca",        "pture",   {my_capture},  0 },
    {"cl",        "oak",     {shield},      -1},
    {"c",         "omputer", {my_computer}, 0 },
    {"da",        "mages",   {my_dcrept},   0 },
    {"destruct",  "",        {my_destruct}, 0 },
    {"do",        "ck",      {my_dock},     0 },
    {"help",      "",        {my_help},     0 },
    {"i",         "mpulse",  {my_impulse},  0 },
    {"l",         "rscan",   {my_lrscan},   0 },
    {"m",         "ove",     {my_warp},     0 },
    {"p",         "hasers",  {my_phaser},   0 },
    {"ram",       "",        {my_warp},     1 },
    {"dump",      "",        {my_dumpgame}, 0 },
    {"r",         "est",     {my_rest},     0 },
    {"sh",        "ield",    {shield},      0 },
    {"s",         "rscan",   {srscan},      0 },
    {"st",        "atus",    {srscan},      -1},
    {"terminate", "",        {my_myreset},  0 },
    {"t",         "orpedo",  {my_torped},   0 },
    {"u",         "ndock",   {my_undock},   0 },
    {"v",         "isual",   {my_visual},   0 },
    {"w",         "arp",     {my_setwarp},  0 },
    {0,           0,         {0},           0 },
};

void myreset(void)
{
    extern jmp_buf env;

    longjmp(env, 1);
}

void play(void)
{
    const struct cvntab *r;

    while (1)
    {
        Move.free = 1;
        Move.time = 0.0;
        Move.shldchg = 0;
        Move.newquad = 0;
        Move.resting = 0;
        skiptonl(0);
        r = getcodpar("\nCommand", Comtab);
        (*r->func)(r->value2);
        events(0);
        attack(0);
        checkcond();
    }
}
