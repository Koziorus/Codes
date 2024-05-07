#include "sorting.h"
#include "generator.h"
#include "tapes.h"

#include <stdio.h> 
#include <time.h> 
#include <math.h>

#define USER_INPUT 24
#define RANDOM_INPUT 25
#define TEST_FILE_INPUT 26

// if defined then file will be printed before and after sorting
#define PRINT_BEFORE_AND_AFTER_SORTING

void print_arr(int* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", arr[i]);
    }
}

// NOTE: records_num is not a program parameter, because it can change dynamically
void print_params()
{
    printf("Program parameters:\n\tMAX_RECORD_SIZE: %d\n\tMAX_RECORD_ELEM_VALUE: %d\n\tMAX_NUMBER_SIZE: %d\n\tMAX_RECORD_ELEM_NUM: %d\n\tFULL_BLOCK_RECORD_NUM: %d\n\tMAX_FIB_NUM: %d\n\n", MAX_RECORD_SIZE, MAX_RECORD_ELEM_VALUE, MAX_NUMBER_SIZE, MAX_RECORD_ELEM_NUM, FULL_BLOCK_RECORD_NUM, MAX_FIB_NUM);
}

#define TEST_NUM 5
#define RECORD_INPUT TEST_FILE_INPUT
#define INITIAL_RECORD_NUM 2

int main()
{
    char prime_table_static[MAX_RECORD_ELEM_VALUE + 1];
    prime_table = prime_table_static;

    print_params();
    srand(time(NULL));
    find_primes(MAX_RECORD_ELEM_VALUE);

    int fib_table[MAX_FIB_NUM];
    fib_arr = fib_table;
    for (int i = 0; i < MAX_FIB_NUM; i++)
    {
        fib_table[i] = 0;
    }

    FILE* test_out;
    fopen_s(&test_out, "test_out.txt", "w");

    int test_phases_num[TEST_NUM] = { 0 };
    int test_runs_num[TEST_NUM] = { 0 };
    int test_disk_reads[TEST_NUM] = { 0 };
    int test_disk_writes[TEST_NUM] = { 0 };
    int test_records_num[TEST_NUM] = { 0 };

    for (int test_index = 0; test_index < TEST_NUM; test_index++)
    {
        printf("---Running test %d---\n", test_index + 1);

        // if the files existed it clears them (WRITE mode) or just opens them (READ mode)
        Tape* tape1 = create_tape("files/tape1.txt", WRITE);
        Tape* tape2 = create_tape("files/tape2.txt", WRITE);
        Tape* tape3 = create_tape("files/tape3.txt", WRITE);
            
        int records_num = INITIAL_RECORD_NUM;
        //records_num = (test_index + 1) * 10;
        
        clock_t record_generation_clock;
        // generate random file
        
        printf("Record generation... ");
        record_generation_clock = clock();

        switch (RECORD_INPUT)
        {
        case USER_INPUT:
        {
            console_input_file(tape3, records_num);
            break;
        }
        case RANDOM_INPUT:
        {
            generate_random_file(tape3, records_num);
            break;
        }
        case TEST_FILE_INPUT:
        {
            char file_path[MAX_FILE_PATH_LEN];
            sprintf_s(file_path, MAX_FILE_PATH_LEN, "test_files/test_file%d.txt", test_index);
            records_num = input_test_file(tape3, file_path);
            break;
        }
        }

        clear_disk_operation_counts(tape3->block_buffer);

        record_generation_clock = clock() - record_generation_clock;
        double record_generation_time = ((double)record_generation_clock) / CLOCKS_PER_SEC;
        printf("%.3fs\n", record_generation_time);

        tape_set_mode(tape3, READ);

#ifdef PRINT_BEFORE_AND_AFTER_SORTING
        print_tape(tape3->block_buffer->assigned_file_path);
#endif 

        int bigger_distr_tape_index;
        printf("Initial distribution...\n");

        clock_t initial_distribution_clock = clock();
        int dummy_runs_num = initial_distribution(tape3, tape1, tape2, &bigger_distr_tape_index, &test_runs_num[test_index]);
        
        // NOTE: in output runs include dummy runs
        test_runs_num[test_index] += dummy_runs_num;
        
        initial_distribution_clock = clock() - initial_distribution_clock;
        double initial_distribution_time = ((double)initial_distribution_clock) / CLOCKS_PER_SEC;
        printf("%.3fs\n", initial_distribution_time);

        // create a container for tapes, where the first one is the biggest input tape and the last one is the output tape
        Tape* tapes[TAPE_NUM];
        tapes[0] = (bigger_distr_tape_index == 0 ? tape1 : tape2);
        tapes[1] = (bigger_distr_tape_index == 0 ? tape2 : tape1);
        tapes[2] = tape3;

        tape_set_mode(tapes[0], READ);
        tape_set_mode(tapes[1], READ);
        tape_set_mode(tapes[2], WRITE);

        printf("Sorting:\n");

        clock_t sorting_clock;
        sorting_clock = clock();
        test_phases_num[test_index] = sort_tapes(tapes, dummy_runs_num);
        sorting_clock = clock() - sorting_clock;

        printf("Sorting time: %.3fs\n", ((double)sorting_clock) / CLOCKS_PER_SEC);

        // the tape with the result
        Tape* end_out_tape = tapes[(TAPE_NUM - ((test_phases_num[test_index] - 1 + 1) % TAPE_NUM)) % TAPE_NUM]; // simillar expression to get the out tape as in sort_tapes() function

#ifdef PRINT_BEFORE_AND_AFTER_SORTING
        tape_set_mode(end_out_tape, READ);
        print_tape(end_out_tape->block_buffer->assigned_file_path);
#endif
        test_records_num[test_index] = records_num;
        test_disk_reads[test_index] = tapes[0]->block_buffer->disk_reads + tapes[1]->block_buffer->disk_reads + tapes[2]->block_buffer->disk_reads;
        test_disk_writes[test_index] = tapes[0]->block_buffer->disk_writes + tapes[1]->block_buffer->disk_writes + tapes[2]->block_buffer->disk_writes;

        delete_tape(&tape1);
        delete_tape(&tape2);
        delete_tape(&tape3);
    }

    for (int i = 0; i < TEST_NUM; i++)
    {
        printf("\nTest %d:\n\trecords: %d\n\truns: %d\n\tphases: %d\n\tdisk reads: %d\n\tdisk writes: %d\n", i + 1, test_records_num[i], test_runs_num[i], test_phases_num[i], test_disk_reads[i], test_disk_writes[i]);

        fprintf(test_out, "%d %d %d %d %d\n", test_records_num[i], test_runs_num[i], test_phases_num[i], test_disk_reads[i], test_disk_writes[i]);
    }

    fclose(test_out);

    
}
