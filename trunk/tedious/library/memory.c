//  File:   memory.c
//  Author: Yuri Panchul

#include <stdlib.h>
#include <p32xxxx.h>

#include "types.h"
#include "memory.h"

static void dump (uint virtual_address, uint size)
{
    uint * start        = (uint *) virtual_address;
    uint * end          = start + size / sizeof (uint);

    uint   block_size   = 1024 / sizeof (uint);
    uint * block_end    = start + block_size;

    uint   column       = 0;
    uint   max_columns  = 64;

    uint * p            = start;
    uint   n;

    printf ("\n");

    for (;;)
    {
        n = 0;

        while (p < block_end)
        {
            if (* p != 0 && * p != ~ 0)
                n ++;

            p ++;
        }

        if (n == 0)
        {
            printf (".");
        }
        else
        {
            uint d = max (1, n * 9 / block_size);
            printf ("%c", '0' + d, n);
        }

        if (p == end)
            break;

        if (++ column == max_columns)
        {
            printf ("\n");
            column = 0;
        }

        block_end += block_size;

        if (block_end > end)
            block_end = end;
    }

    printf ("\n\n");
}

static void region
(
    bool   kernel,
    char * name,
    uint   virtual_address,
    uint   size
)
{
    uint physical_address
      = kernel
        ? virtual_address & ~ 0xE0000000
        : virtual_address +   0x40000000;

    printf ("%-40s  ", name);

    if (size == 0)
        printf ("                                            none\n");
    else
        printf ("%08x-%08x  %08x-%08x  %10u  %7u KB\n",
            virtual_address,  virtual_address  + size - 1,
            physical_address, physical_address + size - 1,
            size, size / 1024);

    // if (size > 0 && size < 1024 * 1024)
    //     dump (virtual_address, size);
}

static int test_variable;

void memory_report (void)
{
    bool no_optional_data_partitions;

    printf ("************  Memory report  ***********");

    printf ("\nFlash memory data cacheability for DMA accesses is %s\n",
            BMXCONbits.BMXCHEDMA ? "ENABLED" : "DISABLED");

    if (BMXCONbits.BMXCHEDMA)
        printf ("(requires cache to have data caching enabled)\n\n");
    else
        printf ("(hits are still read from the cache, but misses do not update the cache)\n\n");

    printf ("Bus error enables: %d %d %d %d %d\n",
            BMXCONbits.BMXERRIXI,
            BMXCONbits.BMXERRICD,
            BMXCONbits.BMXERRDMA,
            BMXCONbits.BMXERRDS,
            BMXCONbits.BMXERRIS);

    printf ("\nNumber of wait states for address setup for data RAM accesses: %d\n",
            BMXCONbits.BMXWSDRM);

    if (BMXCONbits.BMXWSDRM)
        printf ("(slower but useful for debug)\n\n");
    else
        printf ("(fast but has a problem with debugging data breakpoint)\n\n");

    printf ("\nBus Matrix Arbitration Mode: %d\n",
            BMXCONbits.BMXARB);

    switch (BMXCONbits.BMXARB)
    {
        case 0:

            printf ("Debug > Data > Instruction > DMA > Expansion\n");
            printf ("May starve DMA if DMA is used\n\n");
            break;

        case 1:

            printf ("Debug > Data > DMA > Expansion > Instruction\n\n");
            break;

        case 2:

            printf ("Rotating Priority Sequence\n\n");
            break;
    }

    printf ("\nAddress map\n\n");

    no_optional_data_partitions = (BMXDKPBA == 0) || (BMXDUDBA == 0) || (BMXDUPBA == 0);

    printf ("                                          Virtial            Physical                 Size\n");

    region
    (
        true,
        "Boot Flash",
        0xBFC00000,
        BMXBOOTSZ
    );

    region
    (
        true,
        "Kernel Program Flash non-cacheable",
        0xBD000000,
        BMXPUPBA == 0 ? BMXPFMSZ : BMXPUPBA
    );

    region
    (
        true,
        "Kernel Program Flash cacheable",
        0x9D000000,
        BMXPUPBA == 0 ? BMXPFMSZ : BMXPUPBA
    );

    region
    (
        true,
        "Kernel Data RAM - kseg0",
        0x80000000,
        no_optional_data_partitions ? BMXDRMSZ : BMXDKPBA
    );

    region
    (
        true,
        "Kernel Data RAM - kseg1",
        0xA0000000,
        no_optional_data_partitions ? BMXDRMSZ : BMXDKPBA
    );

    region
    (
        true,
        "Kernel Program RAM - kseg0",
        0x80000000 + BMXDKPBA,
        no_optional_data_partitions ? 0 : BMXDUDBA - BMXDKPBA
    );

    region
    (
        true,
        "Kernel Program RAM - kseg1",
        0xA0000000 + BMXDKPBA,
        no_optional_data_partitions ? 0 : BMXDUDBA - BMXDKPBA
    );

    region
    (
        true,
        "Peripheral",
        0xBF800000,
        1024 * 1024
    );

    region
    (
        false,
        "User Program Flash",
        0x7D000000 + BMXPUPBA,
        BMXPUPBA == 0 ? 0 : BMXPFMSZ - BMXPUPBA
    );

    region
    (
        false,
        "User Data RAM",
        0x7F000000 + BMXDUDBA,
        no_optional_data_partitions ? 0 : BMXDUPBA - BMXDUDBA
    );

    region
    (
        false,
        "User Program RAM",
        0x7F000000 + BMXDUPBA,
        no_optional_data_partitions ? 0 : BMXDRMSZ - BMXDUPBA
    );

    printf ("\nTest function : %8x\n", (uint) memory_report);
    printf ("Test variable : %8x\n", (uint) & test_variable);

    printf ("****************************************\n");
}
