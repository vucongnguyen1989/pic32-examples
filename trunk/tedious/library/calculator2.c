//  File:   calculator2.c
//  Author: Yuri Panchul

#include "string.h"
#include "types.h"

#define add    0xa
#define sub    0xb
#define mul    0xc
#define div    0xd
#define sign   0xe
#define equal  0xf

static char   buf [128];
static char * p = buf;

////////////////////////////////////////////////////////////////////////////

static void int_to_string (int n, char * buf)
{
    uint i;

    if (n < 0)
    {
        * buf ++ = '-';
        n = - n;
    }

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            * buf ++ = '0' + n / i % 10;
    }

    * buf = '\0';
}

////////////////////////////////////////////////////////////////////////////

static bool expr1 (int * pn);
static bool expr2 (int * pn);
static bool expr3 (int * pn);

static void expr ()
{
    int n;

    p = buf;

    if (expr1 (& n) && *p == '\0')
    {
        int_to_string (n, buf);
    }
    else
    {
        strcpy (buf, "error @ ");
        int_to_string (p - buf, buf + strlen (buf));
    }

    p = buf;
}

////////////////////////////////////////////////////////////////////////////

static bool expr1 (int * pn)
{
    int op, n1, n2, n;

    if (! expr2 (& n1))
        return false;

    op = * p;

    if (op != '+' && op != '-')
    {
        * pn = n1;
        return true;
    }

    p ++;

    if (! expr1 (& n2))
        return false;

    if (op == '-')
        n2 = - n2;

    n = n1 + n2;

    if (    n1 > 0 && n2 > 0 && n < 0
         || n1 < 0 && n2 < 0 && n > 0)
    {
        return false;
    }

    * pn = n;
    return true;
}

////////////////////////////////////////////////////////////////////////////

static bool expr2 (int * pn)
{
    int op, n1, n2, n;

    if (! expr3 (& n1))
        return false;

    op = * p;

    if (op != '*' && op != '/')
    {
        * pn = n1;
        return true;
    }

    p ++;

    if (! expr2 (& n2))
        return false;

    if (op == '*')
    {
        n = n1 * n2;
 
        if (n2 != 0 && n / n2 != n1)
            return false;
    }
    else
    {
        if (n2 == 0)
            return false;

        n = n1 / n2;
    }

    * pn = n;
    return true;
}

////////////////////////////////////////////////////////////////////////////

static bool expr3 (int * pn)
{
    if (* p == '-' && (p == buf || strchr ("+-*/(", p [-1])))
    {
        p ++;

        if (! expr3 (pn))
            return false;

        * pn = - *pn;
    }
    else if (* p == '(')
    {
        p ++;

        if (! expr1 (pn))
            return false;

        if (* p != ')')
            return false;

        p ++;
    }
    else if (isdigit (* p))
    {
        int n = 0;

        do
        {
            int d  = * p - '0';
            int nn = n * 10 + d;

            if ((nn - d) / 10 != n)
                return false;

            n = nn;
        }
        while (isdigit (* ++ p));

        * pn = n;
    }
    else
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////

char * calculator (char in)
{
    if (in == equal)
    {
        expr (buf);
        return buf;
    }

    switch (in)
    {
    case add  : in = '+';       break;
    case sub  : in = '-';       break;
    case mul  : in = '*';       break;
    case div  : in = '/';       break;
    case sign : in = '-';       break;
    default   : in = '0' + in;  break;
    }

    if (p == buf + sizeof (buf) - 2)
    {
        p = buf;
        return "error";
    }

    * p ++ = in;
    * p    = '\0';
    return p - 1;
}

////////////////////////////////////////////////////////////////////////////

#ifndef __pic32mx__

#include "conio.h"
#include "ctype.h"
#include "stdio.h"

int main1 (void)
{
    for (;;)
    {
        gets (buf);

        if (* buf == '\0')
            break;

        expr (buf);
        puts (buf);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////

int main (void)
{
    char   c;
    char * nl;

    for (;;)
    {
        c = getch ();

        if (strchr ("zq.", c) != NULL)
            break;

        switch (c)
        {
        case '+'  : c = add      ; break;
        case '-'  : c = sub      ; break;
        case '*'  : c = mul      ; break;
        case '/'  : c = div      ; break;
        case '\r' : c = equal    ; break;
        default   : c = c - '0'  ; break;
        }

        nl = c == equal ? "\n" : "";

        printf ("%s%s%s", nl, calculator (c), nl);
    }

    return 0;
}

#endif
