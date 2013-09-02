//  File:   sort.inc
//  Author: Yuri Panchul

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
