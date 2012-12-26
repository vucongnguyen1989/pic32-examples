//  File:   keypad.h
//  Author: Yuri Panchul

#ifndef KEYPAD_H
#define KEYPAD_H

#include "types.h"

void  keypad_init     (bool use_interrupts);
bool  keypad_try_get  (uchar * pa);
uchar keypad_get      (void);

#endif
