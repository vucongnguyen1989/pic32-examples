//  File:   main.c
//  Author: Yuri Panchul

//  I am using optimization level 1 for these measurements

#include <sys/attribs.h>
#include <p32xxxx.h>
#include <plib.h>
#include <xc.h>

#include <stdio.h>

#include "config.h"
#include "types.h"
#include "prefetch_cache.h"

//--------------------------------------------------------------------

#define REPEAT 1000
#define N      16

const volatile int fa [N]
    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

volatile int a [N];

//--------------------------------------------------------------------

__longramfunc__ int sort_working_in_ram (void)
{
    #include "sort.inc"

    return 0;
}

__longramfunc__ int sum_using_for_loop_working_in_ram_using_ram_data (void)
{
    #include "sum_using_for_loop.inc"
}

__longramfunc__ int sum_using_for_loop_working_in_ram_using_flash_data (void)
{
    const volatile int * a = fa;

    #include "sum_using_for_loop.inc"
}

__longramfunc__ int sum_using_flat_sequence_working_in_ram_using_ram_data (void)
{
    #include "sum_using_flat_sequence.inc"
}

__longramfunc__ int sum_using_flat_sequence_working_in_ram_using_flash_data (void)
{
    const volatile int * a = fa;

    #include "sum_using_flat_sequence.inc"
}

//--------------------------------------------------------------------

int sort_working_in_flash (void)
{
    #include "sort.inc"

    return 0;
}

int sum_using_for_loop_working_in_flash_using_ram_data (void)
{
    #include "sum_using_for_loop.inc"
}

int sum_using_for_loop_working_in_flash_using_flash_data (void)
{
    const volatile int * a = fa;

    #include "sum_using_for_loop.inc"
}

int sum_using_flat_sequence_working_in_flash_using_ram_data (void)
{
    #include "sum_using_flat_sequence.inc"
}

int sum_using_flat_sequence_working_in_flash_using_flash_data (void)
{
    const volatile int * a = fa;

    #include "sum_using_flat_sequence.inc"
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

int test_big_iteration = 0;

void test_one_function (int (* f) (void), char * name)
{
    unsigned double_clock_cycles;
    int result;
    int i;

    // Clear all cache statistics

    CHEHIT   = 0;
    CHEMIS   = 0;
    CHEPFABT = 0;

    // Invalidate all cache lines (does not work with some PIC32s)

    for (i = 0; i < 16; i++)
    {
        CHEACC            = 0x80000000 /* Writable */ | i;
        CHETAGbits.LVALID = 0;
    }

    // Clear counter

    _CP0_SET_COUNT (0);

    // printf ("Check counter: %10u\n", _CP0_GET_COUNT ());

    double_clock_cycles = _CP0_GET_COUNT ();
    result = f ();
    double_clock_cycles = _CP0_GET_COUNT () - double_clock_cycles;

    // printf ("%-60.60s : address : %.8X", name, (unsigned) f);

    // printf (" : result : %10u : double clock cycles : %10u\n",
    //     result, double_clock_cycles);

    // printf ("%-58.58s  %10u\n", name, double_clock_cycles);

    printf ("%-60.60s  %4d  %10u\n",
        name, test_big_iteration, double_clock_cycles);

    check_sorting_results ();
    // prefetch_cache_report (false);
}

#define TEST(f)  test_one_function (f, #f);

//--------------------------------------------------------------------

void test_all_functions ()
{
    TEST ( sort_working_in_ram                                       );
    TEST ( sum_using_for_loop_working_in_ram_using_ram_data          );
    TEST ( sum_using_for_loop_working_in_ram_using_flash_data        );
    TEST ( sum_using_flat_sequence_working_in_ram_using_ram_data     );
    TEST ( sum_using_flat_sequence_working_in_ram_using_flash_data   );
    TEST ( sort_working_in_flash                                     );
    TEST ( sum_using_for_loop_working_in_flash_using_ram_data        );
    TEST ( sum_using_for_loop_working_in_flash_using_flash_data      );
    TEST ( sum_using_flat_sequence_working_in_flash_using_ram_data   );
    TEST ( sum_using_flat_sequence_working_in_flash_using_flash_data );

    test_big_iteration ++;
}

//--------------------------------------------------------------------

void main ()
{
    bool alternative_way = true;

    __C32_UART = 1;

    uart_init (9600);

    printf ("******************************************************************************\n");
    printf ("******************************************************************************\n");
    printf ("******************************************************************************\n");
    printf ("*                                                                            *\n");
    printf ("*       %s                                                          *\n", __DATE__);
    printf ("*       %s                                                             *\n", __TIME__);
    printf ("*                                                                            *\n");
    printf ("******************************************************************************\n");

    printf ("******************************************************************************\n");
    printf ("*                                                                            *\n");
    printf ("*       Default state                                                        *\n");
    printf ("*                                                                            *\n");
    printf ("******************************************************************************\n");

    test_all_functions ();
    
    printf ("******************************************************************************\n");
    printf ("*                                                                            *\n");
    printf ("*       Prefetch enabled, cache disabled                                     *\n");
    printf ("*                                                                            *\n");
    printf ("******************************************************************************\n");

    CHECONbits.PREFEN = 3;

    test_all_functions ();

    printf ("******************************************************************************\n");
    printf ("*                                                                            *\n");
    printf ("*       Prefetch disabled, cache enabled                                     *\n");
    printf ("*                                                                            *\n");
    printf ("******************************************************************************\n");

    CHECONbits.PREFEN = 0;

    if (alternative_way)
    {
        CheKseg0CacheOn ();
    }
    else
    {
        unsigned config = _mfc0 (_CP0_CONFIG, _CP0_CONFIG_SELECT);

        config &= ~ _CP0_CONFIG_K0_MASK;
        config |= 3 << _CP0_CONFIG_K0_POSITION;

        _mtc0 (_CP0_CONFIG, _CP0_CONFIG_SELECT, config);
    }

    test_all_functions ();

    printf ("******************************************************************************\n");
    printf ("*                                                                            *\n");
    printf ("*       Prefetch enabled, cache enabled                                      *\n");
    printf ("*                                                                            *\n");
    printf ("******************************************************************************\n");

    CHECONbits.PREFEN = 3;

    test_all_functions ();

    printf ("******************************************************************************\n");
    printf ("*                                                                            *\n");
    printf ("*       Enable data caching                                                  *\n");
    printf ("*                                                                            *\n");
    printf ("******************************************************************************\n");

    CHECONbits.DCSZ = 3;  // Enable data caching with a size of 4 Lines

    test_all_functions ();

    printf ("******************************************************************************\n");
    printf ("*                                                                            *\n");
    printf ("*       Set zero state data RAM access                                       *\n");
    printf ("*                                                                            *\n");
    printf ("******************************************************************************\n");

    if (alternative_way)
        mBMXDisableDRMWaitState ();
    else
        BMXCONbits.BMXWSDRM = 0;

    test_all_functions ();

    printf ("******************************************************************************\n");
    printf ("******************************************************************************\n");
    printf ("******************************************************************************\n");

    for (;;);
}
