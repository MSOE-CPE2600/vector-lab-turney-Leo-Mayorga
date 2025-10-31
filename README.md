# Project: Vector Calculator
# Author: Leonardo Mayorga
# Cource: CPE 2600 - Systems Programming
# Files: main.c, store.c, ops.c, vect.h, Makefile

# Description
The Vector Calculator is a program the performs basic vector math operations:
    - create/replace vectors
    - add and subract existing vectors
    - scale a vector by scalar
    - load/save vectors from/to CSV files

# Version History
    - v1.0 - Original Vector Calculator (max 10 vectors)
    - v1.0 - Added dynamic memory, file I/O, and README.md. This latestest version can grow to hold more than vectors as the user loads and creates them 

# How to Build the Program
run the following commands:
    - make

# How to Run the Program
    - ./minimat
    - ./minimat -h (display help menu)

# Command Supported by the Program
    - CREATE/REPLACE:           name = x y z
    - ADD:                      A + B
    - SUB:                      A - B
    - SCALE BY SCALAR:          A * 5 OR 5 * A
    - STORE OPERATION RESULT:   R = A + B OR R = A * 3
    - SHOW ALL STORED VECTORS:  list
    - RESET STORAGE:            clear
    - LOAD VECTORS FROM CSV:    load filename.csv: 
    - SAVE VECTORS TO CSV:      save filename.csv
    - EXIT PROGRAM:             quit

# Dynamic Memory Usage
    Vector storage is not a fixed array, instead it starts by allocating space for 10 vectors in store_init(). Then when more vectors are added than will fit, store.c calls a grow function that reallocs the array to a bigger size and initializing the new slots, double the original capacity. When exiting the program, the heap memory is freed.