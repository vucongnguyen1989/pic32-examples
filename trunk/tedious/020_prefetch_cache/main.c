//  File:   prefetch_cache.c
//  Author: Yuri Panchul

#include <sys/attribs.h>
#include <p32xxxx.h>
#include <plib.h>
#include <xc.h>

#include "config.h"
#include "types.h"
#include "prefetch_cache.h"

//--------------------------------------------------------------------

#define REPEAT 100
#define N      16

const int fa [N]
    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

int a [N];

//--------------------------------------------------------------------

__longramfunc__ void sort_working_in_sram ()
{
    int i, j, k;

    for (k = 0; k < REPEAT; k ++)
    {
        for (i = 0; i < N; i ++)
            a [i] = fa [i];
        
        for (j = 0; j < N - 1; j ++)
        {
            for (i = 0; i < N - j - 1; i ++)
            {
                if (a [i] < a [i + 1])
                {
                    int t     = a [i];
                    a [i]     = a [i + 1];
                    a [i + 1] = t;
                }
            }
        }
    }
}

//--------------------------------------------------------------------

void sort_working_in_flash ()
{
    int i, j, k;

    for (k = 0; k < REPEAT; k ++)
    {
        for (i = 0; i < N; i ++)
            a [i] = fa [i];
        
        for (j = 0; j < N - 1; j ++)
        {
            for (i = 0; i < N - j - 1; i ++)
            {
                if (a [i] < a [i + 1])
                {
                    int t     = a [i];
                    a [i]     = a [i + 1];
                    a [i + 1] = t;
                }
            }
        }
    }
}

//--------------------------------------------------------------------

void check_sorting_results ()
{
    int i;

    for (i = 0; i < N; i ++)
    {
        // printf ("a [%2d] = %10d : fa [%2d] = %10d\n",
        //     i, a [i], i, fa [i]);
        
        if (a [i] != fa [N - 1 - i])
            printf ("!!! Sorting error !!!\n");
    }
}

//--------------------------------------------------------------------

void main_test ()
{
    unsigned before, after;
    unsigned sram_time, flash_time;

    CHEHIT   = 0;
    CHEMIS   = 0;
    CHEPFABT = 0;

    _CP0_SET_COUNT (0);
    before = _CP0_GET_COUNT ();
    sort_working_in_sram ();
    after  = _CP0_GET_COUNT ();

    printf ("Sorting in SRAM  (in double clock cycles) : %10u - %10u = %10u\n",
        after, before, after - before);

    sram_time = after - before;

    check_sorting_results ();
    prefetch_cache_report();

    CHEHIT   = 0;
    CHEMIS   = 0;
    CHEPFABT = 0;

    _CP0_SET_COUNT (0);
    before = _CP0_GET_COUNT ();
    sort_working_in_flash ();
    after  = _CP0_GET_COUNT ();

    printf ("Sorting in Flash (in double clock cycles) : %10u - %10u = %10u\n",
        after, before, after - before);

    flash_time = after - before;

    check_sorting_results ();
    prefetch_cache_report();

    printf ("Relative execution time: sram  %u / flash %u = %.3g\n",
            sram_time, flash_time, (double) sram_time / flash_time);

    printf ("Relative execution time: flash %u / sram  %u = %.3g\n",
            flash_time, sram_time, (double) flash_time / sram_time);
}

//--------------------------------------------------------------------

void main ()
{
    int pass;

    unsigned before, after;

    __C32_UART = 1;

    uart_init (9600);

    printf ("**********************************************************************\n");
    printf ("**********************************************************************\n");
    printf ("**********************************************************************\n");

    printf ("sort_working_in_sram  : %.8X\n", sort_working_in_sram);
    printf ("sort_working_in_flash : %.8X\n", sort_working_in_flash);

    prefetch_cache_report();

    main_test ();
    
    // Let's enable prefetch
    CHECONbits.PREFEN = 3;

    main_test ();

    // Let's disable prefetch
    CHECONbits.PREFEN = 0;

    // Now let's make KSEG0 cacheable and repeat

    {
        unsigned config = _mfc0 (_CP0_CONFIG, _CP0_CONFIG_SELECT);

        config &= ~ _CP0_CONFIG_K0_MASK;
        config |= 3 << _CP0_CONFIG_K0_POSITION;

        _mtc0 (_CP0_CONFIG, _CP0_CONFIG_SELECT, config);
    }

    main_test ();

    // Let's enable prefetch
    CHECONbits.PREFEN = 3;

    main_test ();

    for (;;);
}
