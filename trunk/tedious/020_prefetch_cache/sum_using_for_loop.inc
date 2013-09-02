//  File:   sum_using_for_loop.inc
//  Author: Yuri Panchul

{
    int i, k, n = 0;

    for (k = 0; k < REPEAT; k ++)
    {
        for (i = 0; i < N; i ++)
            n += a [i];
    }

    return n;
}
