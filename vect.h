/**
 * Filename: vect.h
 * Author: Leo Mayorga 
 * Description: defines vect and constants for math, storage, and printing
 */

#ifndef VECT_H
#define VECT_H

#include <stdbool.h>

#define MAX_VECTS 10
#define VNAME_LEN 16

typedef struct 
{
    char name[VNAME_LEN];   // vector name 
    double v[3];            // x, y, z
    bool in_use;            // storage flag for slot
} vect;

// math 
vect v_add(vect a, vect b);     // add
vect v_sub(vect a, vect b);     // subtract
vect v_scale(vect a, double s); // scale by scalar

// storage
void store_init(void);                          // clear store
int store_find(const char *name);               // find slot index 
int store_add_or_replace(vect value);           // insert or replace
bool store_get(const char *name, vect *out);    // load by name
bool store_set(const char *name, vect value);   // save by name
void store_clear(void);                         // empty everything
void store_list(void);                          // print all vectors

// util
void print_vect(const char *label, const vect *v, int precision);   // print vector

#endif

