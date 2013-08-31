//  File:   flash_cache.c
//  Author: Yuri Panchul

#include <sys/attribs.h>
#include <p32xxxx.h>
#include <plib.h>
#include <xc.h>

#include "config.h"
#include "types.h"
// #include "flash_cache.h"

//--------------------------------------------------------------------

void flash_cache_report (void)
{
    int i;

    printf ("*********  Flash cache report  *********\n");

    printf ("\nCache Coherency setting on a PFM Program Cycle bit:\n"
            "    %s\n\n",
            CHECONbits.CHECOH ?
                "Invalidate all data and instruction lines"
              : "Invalidate all data lines and instruction lines that are not locked");

    printf ("Data Cache Size: ");

    switch (CHECONbits.DCSZ)
    {
    case 3: printf ( "Enable, 4 lines\n" ); break;
    case 2: printf ( "Enable, 2 lines\n" ); break;
    case 1: printf ( "Enable, 1 line\n"  ); break;
    case 0: printf ( "Disable\n"         ); break;
    }

    printf ("\nPredictive Prefetch Cache:\n");

    switch (CHECONbits.PREFEN)
    {
    case 3: printf ( "    Enable for both cacheable and non-cacheable regions\n\n" ); break;
    case 2: printf ( "    Enable for non-cacheable regions only\n\n"               ); break;
    case 1: printf ( "    Enable for cacheable regions only\n\n"                   ); break;
    case 0: printf ( "    Disable\n\n"                                             ); break;
    }

    printf ("PFM Access Time: %d SYSCLK wait states\n", CHECONbits.PFMWS);

    printf ("\nCache lines:\n\n");

    for (i = 0; i < 16; i ++)
    {
        unsigned address;

        // CHEACCbits.CHEWEN = 0;  // Write enable
        // CHEACCbits.CHEIDX = i;  // Cache Line Index

        CHEACC = i;

        if (CHETAGbits.LTAGBOOT)
            address = 0x1D000000 + (CHETAGbits.LTAG << 4);
        else
            address = 0x1FC00000 + (CHETAGbits.LTAG << 4);

        printf ("    %2d ", i);

        if (! CHETAGbits.LVALID)
        {
            printf ("\n");
            continue;
        }

        printf ("%8X %s %s %s mask: %4X   %.8X:%.8X:%.8X:%.8X\n",
                address,
                CHETAGbits.LVALID ? "valid"       : "     ",
                CHETAGbits.LLOCK  ? "locked"      : "      ",
                CHETAGbits.LTYPE  ? "instruction" : "data       ",
                CHEMSK,
                CHEW0, CHEW1, CHEW2, CHEW3);
    }

    printf ("\nLRU: %.8X\n", CHELRU);

    printf ("\nCounters\n\n");

    printf ("    Hit            : %10d\n", CHEHIT );
    printf ("    Miss           : %10d\n", CHEMIS );
    printf ("    Prefetch Abort : %10d\n", CHEPFABT  );

    printf ("\n");
    printf ("******  End of flash cache report  *****\n\n");
}

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
    flash_cache_report();

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
    flash_cache_report();

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

    flash_cache_report();

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
