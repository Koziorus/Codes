#pragma once

#include <stdio.h>
#include <stdlib.h>

//PARAM

//MAX_RECORD_SIZE should include size of an index (if records are indexed)
#define MAX_RECORD_SIZE 200
#define MAX_RECORD_ELEM_VALUE 1000

// NOTE: MAX_RECORD_ELEM_VALUE is connected to MAX_NUMBER_SIZE 
// TODO: make it so they're dynamically connected
#define MAX_NUMBER_SIZE 5
#define MAX_RECORD_ELEM_NUM 15

#define FULL_BLOCK_RECORD_NUM 5
#define MAX_FILE_PATH_LEN 100

// structure of a file:
/*
record_1_index record_1_elem_0 record_1_elem_1 record_1_elem_2 ...
record_2_index ...
...
*/

typedef struct BlockBuff
{
	// NOTE: block buffer records are fully allocated on stack
	const char records[FULL_BLOCK_RECORD_NUM][MAX_RECORD_SIZE];
	const char assigned_file_path[MAX_FILE_PATH_LEN];
	int record_num; // says how full a block buffer is
	int capacity; // current capacity (usually its FULL_BLOCK_RECORD_NUM)
	FILE* file;
	int disk_reads;
	int disk_writes;
} BlockBuff;

void create_file(const char* path);

const char* read_record(BlockBuff* block_b);

void write_record(BlockBuff* block_b, const char* record_content);

void initialize_block_buffer(BlockBuff* block_b, const char* filepath);

void clear_block_buffer(BlockBuff* block_b);

void flush_block_buffer(BlockBuff* block_b);

void clear_disk_operation_counts(BlockBuff* block_b);

// TODO: make sure that defines with "SIZE" in the name refer to the number of bytes (char lenghts) and not some number of elements (for example: BLOCK_SIZE should mean how many chars it has and not how many records it holds)