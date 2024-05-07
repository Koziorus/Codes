#pragma once

#include "file_operations.h"
#include "tapes.h"

//create a record with an index and random numbers after it: "INDEX NUM1 NUM2 NUM3 ..."
void generate_record(int size, const char* record_content);

void generate_random_file(Tape* tape, int records_num);

void input_record(FILE* stream, int size, const char* record_content);

void console_input_file(Tape* tape, int records_num);

int input_test_file(Tape* tape, const char* test_file_path);