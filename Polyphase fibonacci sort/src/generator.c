#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

//create a record with an index and random numbers after it: "INDEX NUM1 NUM2 NUM3 ..."
void generate_record(int size, const char* record_content)
{
    if (!(size <= MAX_RECORD_ELEM_NUM))
    {
        system("color c");
        fprintf(stderr, "Number of elements in the record is too big! (should be %d)", MAX_RECORD_ELEM_NUM);
        exit(1);
    }

    //generating random numbers in a interval <1, MAX_RECORD_ELEM_VALUE>
    int random_numbers[MAX_RECORD_ELEM_NUM];
    
    for (int i = 0; i < size; i++)
    {
        int random_number = (rand() % MAX_RECORD_ELEM_VALUE) + 1; // <1, MAX_RECORD_ELEM_VALUE>
        random_numbers[i] = random_number;
    }

    //writing the numbers into the record
    strcpy(record_content, ""); // POST-INDEXING-REMOVAL
    for (int i = 0; i < size; i++)
    {
        if (i != 0)
        {
            sprintf_s(record_content, MAX_RECORD_SIZE, "%s ", record_content);
        }

        sprintf_s(record_content, MAX_RECORD_SIZE, "%s%d", record_content, random_numbers[i]);
    }
}

// tape needs to be in WRITE mode
void generate_random_file(Tape* tape, int records_num)
{
    if (tape->mode != WRITE)
    {
        system("color c");
        fprintf(stderr, "Tape should be in WRITE mode for random generation!");
        exit(1);
    }

    for (int i = 0; i < records_num; i++)
    {
        const char record[MAX_RECORD_SIZE];
        generate_record(MAX_RECORD_ELEM_NUM, record);
        write_to_tape(tape, record);
    }
    flush_block_buffer(tape->block_buffer);
}

void input_record(FILE* stream, int size, const char* record_content)
{
    if (!(size <= MAX_RECORD_ELEM_NUM))
    {
        system("color c");
        fprintf(stderr, "Number of elements in the record is too big! (should be %d)", MAX_RECORD_ELEM_NUM);
        exit(1);
    }

    int input_numbers[MAX_RECORD_ELEM_NUM];
    for (int i = 0; i < size; i++)
    {
        fscanf_s(stream, "%d", &input_numbers[i]);
    }

    //writing the numbers into the record
    strcpy(record_content, ""); // POST-INDEXING-REMOVAL
    for (int i = 0; i < size; i++)
    {
        if (i != 0)
        {
            sprintf_s(record_content, MAX_RECORD_SIZE, "%s ", record_content);
        }

        sprintf_s(record_content, MAX_RECORD_SIZE, "%s %d", record_content, input_numbers[i]);
    }
}

// tape needs to be in WRITE mode
void console_input_file(Tape* tape, int records_num)
{
    if (tape->mode != WRITE)
    {
        system("color c");
        fprintf(stderr, "Tape should be in WRITE mode for inputting records!");
        exit(1);
    }

#ifdef USER_INPUT_INDEX_RECORDS
    printf("\nInput %d records (USER_INPUT_INDEX_RECORDS enabled) -> index:\n", records_num);
#else
    printf("\nInput %d records (USER_INPUT_INDEX_RECORDS disabled) (%d elements each):\n", records_num, MAX_RECORD_ELEM_NUM);
#endif

    for (int i = 0; i < records_num; i++)
    {
        const char record[MAX_RECORD_SIZE];
        input_record(stdin, MAX_RECORD_ELEM_NUM, record);
        write_to_tape(tape, record);
    }
    flush_block_buffer(tape->block_buffer);
}

int input_test_file(Tape* tape, const char* test_file_path)
{
    /* test file structure:
    "test in this file (T)/random generation (R)" "number of records" "number of elements of a record"
    record1
    record2
    ...

    for example:
    T 5 15
    758 25 888 186 985 297 395 633 507 184 752 383 951 27 253
    363 454 181 4 516 749 529 17 339 637 784 105 489 884 452
    374 523 21 798 369 357 595 206 599 981 428 209 262 962 380
    351 209 533 679 213 955 142 243 257 561 498 716 413 883 635
    810 685 620 313 849 716 575 320 72 476 749 161 852 580 788
    */

    if (tape->mode != WRITE)
    {
        system("color c");
        fprintf(stderr, "Tape should be in WRITE mode for inputting records!");
        exit(1);
    }

    FILE* test_file;
    fopen_s(&test_file, test_file_path, "r");

    char input_option;
    int records_num, record_elem_num;
    
    // record_index_elem_num is ignored if input_option is set to random generation
    // in fscanf_s (in contrast to fscanf) when using %c or %s ... you have to pass buffer count right after the variable that receives the input (here: there's %c and then &input_option and 1 for size of char)
    fscanf_s(test_file, "%c %d %d", &input_option, 1, &records_num, &record_elem_num);

    if (record_elem_num != MAX_RECORD_ELEM_NUM)
    {
        system("color c");
        fprintf(stderr, "Test file %s has a wrong number of elements of records!", test_file_path);
        fclose(test_file);
        exit(1);
    }

    if (input_option == 'R')
    {
        generate_random_file(tape, records_num);
    }
    else if (input_option == 'T')
    {
        for (int i = 0; i < records_num; i++)
        {
            const char record[MAX_RECORD_SIZE];
            input_record(test_file, record_elem_num, record);
            write_to_tape(tape, record);
        }
        flush_block_buffer(tape->block_buffer);
    }

    fclose(test_file);

    return records_num;
}