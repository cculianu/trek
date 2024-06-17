cmake_minimum_required(VERSION 3.5)

project(trek LANGUAGES C)

add_executable(trek
    # Headers
    getpar.h trek.h trek_funcs.h

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

target_compile_definitions(trek PRIVATE -Dlint)