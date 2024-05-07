#pragma once

#include "file_operations.h"

#define READ 0
#define WRITE 1

// uninitialized mode (means that the file has not been opened yet)
#define NONE 2

#define EMPTY_TAPE 11

// OPTION
#define PRINT_WITH_INDICES

typedef struct Tape
{
    BlockBuff* block_buffer;
    int mode;
} Tape;

char* prime_table;

// changes tape mode (read / write)
void tape_set_mode(Tape* tape, char mode);

// creates a tape with its file and block buffer
Tape* create_tape(const char* filepath, char mode);

void delete_tape(Tape** tape);

void write_to_tape(Tape* tape, const char* record);

int read_from_tape(Tape* tape, const char* record);

int get_index(const char* record);

//print primes in an interval <0, max_value> (created from eratosthenes sieve)
void print_primes(int max_value);

//creating prime table using eratosthenes sieve
void find_primes(int max_value);

//number of primes present in an array of numbers
int get_number_of_primes(int* number_arr, int n);

void print_tape(const char* filepath);