#pragma once

#include "generator.h"
#include "helper.h"
#include "tapes.h"

// OPTION
#define PRINT_AFTER_PHASE

//PARAM

#define MAX_FIB_NUM 50
// number of distribution tapes
#define DISTR_TAPE_NUM 2
#define TAPE_IN_A 0
#define TAPE_IN_B 1

// number of all tapes
#define TAPE_NUM 3

#define SORT_END 13
#define SORT_CONTINUE 14

// array with fibonnacii sequenece (fib[0] = 0, fib[1] = fib[2] = 1)
int* fib_arr;

int sort_phase(Tape* in_A, Tape* in_B, Tape* out, int phase_index, const char* carry_record, int* dummy_runs);

int initial_distribution(Tape* in, Tape* out_a, Tape* out_b, int* bigger_tape_index, int* overall_runs);

int sort_tapes(Tape* tapes[TAPE_NUM], int dummy_runs_num_in);

int fib(int n);