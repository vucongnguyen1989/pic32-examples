//  File:   sum_using_flat_sequence.inc
//  Author: Yuri Panchul

#define FLAT_SEQUENCE                        \
    n =                                      \
          a [ 0] + a [ 1] + a [ 2] + a [ 3]  \
        + a [ 4] + a [ 5] + a [ 6] + a [ 7]  \
        + a [ 8] + a [ 9] + a [10] + a [11]  \
        + a [12] + a [13] + a [14] + a [15];

{
    int i, k, n = 0;

    for (k = 0; k < REPEAT / 8; k ++)
    {

        FLAT_SEQUENCE
        FLAT_SEQUENCE
        FLAT_SEQUENCE
        FLAT_SEQUENCE
        FLAT_SEQUENCE
        FLAT_SEQUENCE
        FLAT_SEQUENCE
        FLAT_SEQUENCE
    }

    return n;
}

#undef FLAT_SEQUENCE
