//  File:   spi.h
//  Author: Yuri Panchul

#ifndef SPI_H
#define SPI_H

#include "types.h"

void  spi_init           (uint baud);

uchar spi_put_get_char   (uchar c);

uchar spi_get_char       (void);

void  spi_put_char       (uchar   c);
void  spi_put_new_line   (void     );
void  spi_put_str        (uchar * s);
void  spi_put_dec        (uint    n);
void  spi_put_hex_digit  (uint    n);
void  spi_put_hex_byte   (uchar   n);
void  spi_put_hex        (uint    n);

#endif
