/**
 * Filename: ops.c
 * Author: Leo Mayorga 
 * Description: computes each vector operation
 */

#include "vect.h"

// performs the addition of two vectors
vect v_add(vect a, vect b)
{
    vect r = a;
    r.v[0] = a.v[0] + b.v[0];
    r.v[1] = a.v[1] + b.v[1];
    r.v[2] = a.v[2] + b.v[2];
    
    return r;
}

// performs the subtraction of two vectors
vect v_sub(vect a, vect b)
{
    vect r = a;
    r.v[0] = a.v[0] - b.v[0];
    r.v[1] = a.v[1] - b.v[1];
    r.v[2] = a.v[2] - b.v[2];

    return r;
}

// performs the multiplication of a vector and a number
vect v_scale(vect a, double s)
{
    vect r = a;
    r.v[0] = a.v[0] * s;
    r.v[1] = a.v[1] * s;
    r.v[2] = a.v[2] * s;

    return r;
}
