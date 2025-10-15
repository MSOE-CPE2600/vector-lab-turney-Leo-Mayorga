/**
 * Filename: store.c
 * Author: Leo Mayorga
 * Description: storage of fixed size 10 for our vectors. Supports init/clear, 
 * insert/replace by name, lookup, listing, and formatted printing.
 */

#include "vect.h"
#include <stdio.h>
#include <string.h>

static vect g_store[MAX_VECTS];

// initiate
// clear store to defaults
void store_init(void)
{
    for (int i = 0; i < MAX_VECTS; i++)
    {
        g_store[i].in_use = false;
        g_store[i].name[0] = '\0';
        g_store[i].v[0] = 0.0;
        g_store[i].v[1] = 0.0;
        g_store[i].v[2] = 0.0;
    }
}

// find the first free slot
static int first_empty(void)
{
    for (int i = 0; i < MAX_VECTS; i++)
    {
        if (!g_store[i].in_use)
        {
            return i; // return the idex of the open slot
        }
    }
    return -1;
}

// find index by name
int store_find(const char *name)
{
    for (int i = 0; i < MAX_VECTS; i++)
    {
        // if it is in use AND the name matches, return that index
        if (g_store[i].in_use && strcmp(g_store[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

// add or overwrite
int store_add_or_replace(vect value)
{
    int idx = store_find(value.name);
    if (idx >= 0)
    {
        g_store[idx] = value;           // store at that index
        g_store[idx].in_use = true;     // update to in use
        return idx;
    }
    idx = first_empty();                // update idx with newest empty index
    if (idx < 0)
    {
        return -1;
    }
    g_store[idx] = value;
    g_store[idx].in_use = true;
    return idx;
}

// copy out by name
bool store_get(const char *name, vect *out)
{
    int idx = store_find(name);     // get the index of that vector
    if (idx < 0)
    {
        return false;
    }
    *out = g_store[idx];
    return true;
}

// save under a specific name
bool store_set(const char *name, vect value)
{
    strncpy(value.name, name, VNAME_LEN - 1); // copies the name
    value.name[VNAME_LEN - 1] = '\0';
    int idx = store_add_or_replace(value);
    return idx >= 0;
}

// clear entire store
void store_clear(void)
{
    store_init();
}

// print stored vectors
void store_list(void)
{
    int count = 0;
    for (int i = 0; i < MAX_VECTS; i++)
    {
        if (!g_store[i].in_use)
        {
            continue;
        }
        count++;
        printf("%s = %.3f   %.3f   %.3f\n",
               g_store[i].name, g_store[i].v[0], g_store[i].v[1], g_store[i].v[2]);
    }
    if (count == 0)
    {
        puts("(no vectors stored)");
    }
}

// format the print
void print_vect(const char *label, const vect *v, int precision)
{
    if (precision < 0 || precision > 10)
    {
        precision = 3;
    }
    char fmt[64];
    // e.g., "%s = %.3f   %.3f   %.3f\n"
    snprintf(fmt, sizeof(fmt), "%%s = %%.%df   %%.%df   %%.%df\n",
             precision, precision, precision);
    printf(fmt, label, v->v[0], v->v[1], v->v[2]);
}
