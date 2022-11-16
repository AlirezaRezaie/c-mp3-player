#include <stdio.h>
#include <unistd.h>
#include <vlc/vlc.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

#pragma once

// GLOBALS
struct arg_struct {
    void * mp;
    bool running;
};

extern char ch;

extern void* adj_vol(void *arguments);

