/**
 * Filename: main.c
 * Author: Leo Mayorga
 * Desctiption: main function that parses commands, validates operands, and executes the 
 * math or storage functions. Handles errors and prints results
 */

#include "vect.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 256

// print help text
static void help(void)
{
    printf("minimat (Vector Calculator)\n");
    printf("Help: minimat [-h]\n");
    printf("Commands:\n");
    printf("  name = x y z        (or with commas)  create/replace vector\n");
    printf("  A + B | A - B | A * k | k * A         compute and display\n");
    printf("  R = <operation>                       compute and assign to R\n");
    printf("  name                                  display the vector\n");
    printf("  list                                  list stored vectors\n");
    printf("  clear                                 clear storage\n");
    printf("  quit                                  exit\n");
    printf("Note: spaces around = and operators are required.\n");
}

// trim leading and trailing spaces
static char *trim(char *s)
{
    while (isspace((unsigned char)*s))
    {
        s++;
    }
    if (*s == 0)
    {
        return s;
    }
    char *e = s + strlen(s) - 1;
    while (e > s && isspace((unsigned char)*e))
    {
        *e-- = '\0';
    }
    return s;
}

// parse double
static int is_number(const char *s, double *out)
{
    char *end = NULL;
    double val = strtod(s, &end);
    if (end && *trim(end) == '\0')
    {
        if (out)
        {
            *out = val;
        }
        return 1;
    }
    return 0;
}

// parse 3 numbers
static int parse_three(const char *RIGHT, double out[3])
{
    // accept: "1 2 3" OR "1, 2, 3" OR "1,2,3"
    char buf[LINE_MAX];
    strncpy(buf, RIGHT, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    for (char *p = buf; *p; ++p)
    {
        if (*p == ',')
        {
            *p = ' ';
        }
    }

    int i = 0;
    char *tok = strtok(buf, " \t");
    while(tok && i < 3)
    {
        double val;
        if (!is_number(tok, &val))
        {
            return 0;
        }
        out[i++] = val;
        tok = strtok(NULL, " \t");
    }
    return i == 3;
}

// split at " = "
static int parse_assign(char *line, char **LEFT, char **RIGHT)
{
    // expect name and '='
    char *eq = strstr(line, " = ");
    if (!eq)
    {
        return 0;
    }
    *eq = '\0';
    *LEFT = trim(line);
    *RIGHT = trim(eq + 3);
    return **LEFT && **RIGHT;
}

// split 
static int parse_op(char *RIGHT, char **a, char *op, char **b)
{
    // expect "A + B" / "A - B" / "A * k" / "k * A" (spaces required)
    const char *ops = "+-*";
    for (const char *o = ops; *o; ++o)
    {
        char pat[4];
        pat[0] = ' ';
        pat[1] = *o;
        pat[2] = ' ';
        pat[3] = '\0';

        char *mid = strstr(RIGHT, pat);
        if (!mid)
        {
            continue;
        }
        *mid = '\0';
        *a = trim(RIGHT);
        *b = trim(mid + 3);
        *op = *o;
        return **a && **b;
    }
    return 0;
}

// print vector by name
static void do_display_name(const char *name)
{
    vect v;
    if (!store_get(name, &v))
    {
        printf("Vector '%s' does not exist.\n", name);
        return;
    }
    print_vect(v.name, &v, 3);
}

int main(int argc, char **argv)
{
    // handle "-h"
    if (argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        help();             // show help message
        return 0;           // exit after help
    }

    store_init();           // initialize storage

    char line[LINE_MAX];
    while (1)
    {
        printf("minimat> ");
        if (!fgets(line, sizeof(line), stdin))
        {
            break;
        }

        char *s = trim(line);   // trim input
        if (*s == '\0')         // ignore if empty
        {
            continue;
        }

        // simple commands
        if (strcmp(s, "quit") == 0)     // exit the program
        {
            break;
        }
        if (strcmp(s, "clear") == 0)    // clear the storage
        {
            store_clear();
            continue;
        }
        if (strcmp(s, "list") == 0)     // list vectors
        {
            store_list();
            continue;
        }

        char *LEFT = NULL;                      // left - name
        char *RIGHT = NULL;                     // right - expression
        if (parse_assign(s, &LEFT, &RIGHT))     // check "name = ..."
        {
            // Case 1: literal components
            double triple[3];                   // holds x y z
            if (parse_three(RIGHT, triple))     // parse numbers
            {
                vect v = {0};                               // new vector
                strncpy(v.name, LEFT, VNAME_LEN - 1);       // set name
                v.name[VNAME_LEN - 1] = '\0';
                v.v[0] = triple[0];                         // set x
                v.v[1] = triple[1];                         // set y
                v.v[2] = triple[2];                         // set z

                if (!store_set(LEFT, v))                 // save vector
                {
                    printf("Memory full. Use 'clear' to reset.\n");
                }
                else
                {
                    print_vect(LEFT, &v, 3);             // print saved
                }
                continue;
            }

            // Case 2: operation on RIGHT (A + B, A - B, A * k, k * A)
            char *a = NULL;             // left operand
            char *b = NULL;             // right operand
            char op = 0;                // operator

            if (!parse_op(RIGHT, &a, &op, &b))    // parse operator
            {
                printf("Invalid assignment expression.\n");
                continue;
            }

            vect A, B, R;               // operands/result
            double num;                 // scalar value

            // add or subtract
            if (op == '+' || op == '-')     
            {
                // load both vectors
                if (!store_get(a, &A) || !store_get(b, &B))
                {
                    printf("Operands must be existing vectors.\n");
                    continue;
                }
                // compute
                R = (op == '+') ? v_add(A, B) : v_sub(A, B);
            }
            else if (op == '*')     // scale
            {
                int a_is_num = is_number(a, &num);  // check left scalar
                int b_is_num = is_number(b, &num);  // check right scalar
                if (a_is_num && !b_is_num)          // k * A
                {
                    if (!store_get(b, &A))          // load A
                    {
                        printf("Vector operand not found.");    
                        continue;
                    }
                    R = v_scale(A, num);            // scale A
                }
                else if (!a_is_num && b_is_num)     // A * K
                {
                    if (!store_get(a, &A))          // load A
                    {
                        printf("Vector operand not found.\n");
                        continue;
                    }
                    R = v_scale(A, num);            // scale A
                }
                else
                {
                    printf("One side must be a scalar, the other a vector.\n");
                    continue;
                }
            }
            else    // bad operation
            {
                printf("Unsupported operator.\n");
                continue;
            }

            // assign to LEFT name 
            strncpy(R.name, LEFT, VNAME_LEN - 1);   // set result name
            R.name[VNAME_LEN - 1] = '\0';
            if (!store_set(LEFT, R))                // save result
            {
                printf("Memory full. Use 'clear' to reset.\n");
                continue;
            }
            print_vect(LEFT, &R, 3);                // result
            continue;
        }

        // no assignment: maybe "A + B", "A - B", "A * k", "k * A", or just "name"
        char *a = NULL;     // left token 
        char *b = NULL;     // right token
        char op = 0;        // operator token

        if (parse_op(s, &a, &op, &b))   // parse operator
        {
            vect A, B, R;               // operands/result
            double num;                 // scalar

            // add or subtract
            if (op == '+' || op == '-')
            {
                if (!store_get(a, &A) || !store_get(b, &B))     // load both
                {
                    printf("Operands must be existing vectors.\n");
                    continue;
                }
                R = (op == '+') ? v_add(A, B) : v_sub(A, B);    // compute
                print_vect("ans", &R, 3);                       // show answer
            }
            else if (op == '*')         // scale
            {
                int a_is_num = is_number(a, &num);  // left scalar?
                int b_is_num = is_number(b, &num);  // right scalar?
                if (a_is_num && !b_is_num)          // K * A
                {
                    if (!store_get(b, &A))
                    {
                        printf("Vector operand not found.");
                        continue;
                    }
                    R = v_scale(A, num);            // compute
                }
                else if (!a_is_num && b_is_num)     // K * A
                {
                    if (!store_get(a, &A))
                    {
                        printf("Vector operand not found.");
                        continue;
                    }
                    R = v_scale(A, num);            // compute
                }
                else
                {
                    printf("One side must be a scalar, the other a vector.\n");
                    continue;
                }
                print_vect("ans", &R, 3);           // show answer
            }
            else
            {
                printf("Unsupported operator.");    // edge case for unsupported opreations
            }
            continue;
        }

        do_display_name(s);     // show stored vector
    }

    return 0;
}
