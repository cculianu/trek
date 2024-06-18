cmake_minimum_required(VERSION 3.12)

project(trek LANGUAGES C)

add_executable(trek
    # Headers
    getpar.h trek.h

    # Sources
    abandon.c
    attack.c
    autover.c
    capture.c
    cgetc.c
    check_out.c
    checkcond.c
    compkl.c
    computer.c
    damage.c
    damaged.c
    dcrept.c
    destruct.c
    dock.c
    dumpgame.c
    dumpme.c
    dumpssradio.c
    events.c
    externs.c
    getcodi.c
    getpar.c
    help.c
    impulse.c
    initquad.c
    kill.c
    klmove.c
    lose.c
    lrscan.c
    main.c
    move.c
    nova.c
    out.c
    phaser.c
    play.c
    ram.c
    ranf.c
    rest.c
    schedule.c
    score.c
    setup.c
    setwarp.c
    shield.c
    snova.c
    srscan.c
    systemname.c
    torped.c
    utility.c
    visual.c
    warp.c
    win.c
)

# Check that target machine has arc4random() (with Linux-specific hack to use libbsd).
cmake_policy(PUSH)
cmake_policy(SET CMP0075 NEW)
include(CheckCSourceRuns)
check_c_source_runs("
#include <stdlib.h>
int main(void) { return (int)(arc4random() + arc4random_uniform(10u)) & 0; }
"
    HAVE_ARC4RANDOM
)
if(NOT HAVE_ARC4RANDOM)
    # On Linux, we must include bsd/stdlib.h and link to libbsd, so test that works.
    find_library(LIB_BSD bsd)
    if(LIB_BSD)
        set(CRL_ORIG "${CMAKE_REQUIRED_LIBRARIES}")
        set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES};bsd")
        check_c_source_runs("
#include <stdlib.h>
#include <bsd/stdlib.h>
int main(void) { return (int)(arc4random() + arc4random_uniform(10u)) & 0; }
"
            HAVE_ARC4RANDOM_BSD
        )
        set(CMAKE_REQUIRED_LIBRARIES "${CRL_ORIG}")
        if(HAVE_ARC4RANDOM_BSD)
            set(HAVE_ARC4RANDOM ON)
            set(USE_LIBBSD ON)
        endif()
    endif()
endif()
cmake_policy(POP)

if(HAVE_ARC4RANDOM)
    target_compile_definitions(trek PRIVATE -DHAVE_ARC4RANDOM)
    if(USE_LIBBSD)
        target_compile_definitions(trek PRIVATE -DUSE_LIBBSD)
        target_link_libraries(trek PRIVATE bsd)
    endif()
endif()

include(CheckIncludeFile)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
    target_compile_definitions(trek PRIVATE -DHAVE_UNISTD_H)
endif()

find_library(LIB_M m)
if(LIB_M)
    target_link_libraries(trek PRIVATE m)
endif()

if(WIN32)
    # On Windows, define this when compiling to not get the warnings about using strncpy(), etc.
    target_compile_definitions(trek PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()
