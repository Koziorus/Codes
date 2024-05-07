#include "tapes.h"

//print primes in an interval <0, max_value> (created from eratosthenes sieve)
void print_primes(int max_value)
{
    for (int i = 0; i <= max_value; i++)
    {
        if (prime_table[i] == 0)
        {
            printf("%d ", i);
        }
    }
    printf("\n");
}

//creating prime table using eratosthenes sieve
void find_primes(int max_value)
{
    for (int i = 0; i <= max_value; i++)
    {
        prime_table[i] = 0;
    }

    prime_table[0] = 1;
    prime_table[1] = 1;
    for (int i = 2; i * i <= max_value; i++)
    {
        if (prime_table[i] == 0)
        {
            for (int j = i * i; j <= max_value; j += i)
            {
                prime_table[j] = 1;
            }
        }
    }
}

//number of primes present in an array of numbers
int get_number_of_primes(int* number_arr, int n)
{
    int primes_num = 0;
    for (int i = 0; i < n; i++)
    {
        if (prime_table[number_arr[i]] == 0)
        {
            primes_num++;
        }
    }

    return primes_num;
}

// changes tape mode (read / write)
void tape_set_mode(Tape* tape, char mode)
{
    // clear block buffer
    clear_block_buffer(tape->block_buffer);

    if (tape->mode != NONE)
    {
        fclose(tape->block_buffer->file);
    }

    tape->mode = mode;
    if (mode == READ)
    {
        fopen_s(&tape->block_buffer->file, tape->block_buffer->assigned_file_path, "r");
    }
    else if (mode == WRITE)
    {
        fopen_s(&tape->block_buffer->file, tape->block_buffer->assigned_file_path, "w");
    }
}

// creates a tape with its file and block buffer
Tape* create_tape(const char* filepath, char mode)
{
    Tape* tape = (Tape*)malloc(sizeof(Tape));
    tape->block_buffer = (BlockBuff*)malloc(sizeof(BlockBuff));
    initialize_block_buffer(tape->block_buffer, filepath);
    //NOTE: that disk_reads and disk_writes are not cleared in initialize_block_buffer() nor in clear_block_buffer()
    tape->block_buffer->disk_reads = 0;
    tape->block_buffer->disk_writes = 0;
    create_file(filepath); // first create and only then open
    tape->mode = NONE; // setting the mode to NONE to notify the tape_set_mode that it shouldn't close the file stream, because it is not opened yet
    tape_set_mode(tape, mode);

    return tape;
}

// NOTE: that the address of a pointer to a tape is passed
// NOTE: does not delete the file associated with the tape
void delete_tape(Tape** tape)
{
    FILE* file = (*tape)->block_buffer->file;

    fclose(file);

    free((*tape)->block_buffer);

    // NOTE: if other buffer is somehow using this file at the same time, the previous buffer may close this file after the later opens it

    free(*tape);
}

void write_to_tape(Tape* tape, const char* record)
{
    if (tape->mode != WRITE)
    {
        system("color c");
        fprintf(stderr, "To write the tape needs to be in WRITE mode!");
        exit(1);
        return;
    }

    write_record(tape->block_buffer, record);
}

int read_from_tape(Tape* tape, const char* record)
{
    if (tape->mode != READ)
    {
        system("color c");
        fprintf(stderr, "To read the tape needs to be in READ mode!");
        exit(1);
        return -1;
    }

    const char* in_record = read_record(tape->block_buffer);
    
    if (in_record == NULL)
    {
        return EMPTY_TAPE;
    }

    strcpy(record, in_record);

    return 0;
}

int get_index(const char* record)
{
    // POST-INDEXING-REMOVAL
    // TODO: sscanf from record into record_elements[] and then use get_number_of_primes() 

    int numbers[MAX_RECORD_ELEM_NUM] = { 0 };
    const char* record_ptr = record;

    // get numbers from record
    for (int i = 0; i < MAX_RECORD_ELEM_NUM; i++)
    {
        sscanf_s(record_ptr, "%d", &numbers[i]);
        
        // get to next number
        record_ptr = strchr(record_ptr, ' ');

        // if end of record
        if (record_ptr == NULL)
        {
            break; 
        }

        record_ptr++; 
    }
    
    int record_index = get_number_of_primes(numbers, MAX_RECORD_ELEM_NUM);

    return record_index;
}

void print_tape(const char* filepath)
{
    FILE* file;
    fopen_s(&file, filepath, "r");
    const char record[MAX_RECORD_SIZE];

    printf("\n");

    while (fgets(record, MAX_RECORD_SIZE + 2, file) != NULL)
    {
#ifdef PRINT_WITH_INDICES
        printf("[%d] ", get_index(record));
#endif
        printf("%s", record);
    }

    printf("\n");

    fclose(file);
}