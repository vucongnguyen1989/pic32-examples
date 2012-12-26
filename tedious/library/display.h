//  File:   display.h
//  Author: Yuri Panchul

#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

void  display_init       (void);
void  display_char       (uchar   c);
void  display_new_line   (void     );
void  display_str        (uchar * s);
void  display_dec        (uint    n);
void  display_hex_digit  (uint    n);
void  display_hex_byte   (uchar   n);
void  display_hex        (uint    n);

#endif
