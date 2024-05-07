#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <intrin.h>

typedef enum Bool
{
	FALSE = 0,
	TRUE = 1
} Bool;

#define MAX_FILE_PATH_LEN 100
#define TRASH_SIZE 10000
#define ONE_BYTE_SIZE 1

// sizes of data in files
// PARAM
typedef uint16_t key_type;
typedef uint16_t page_num_type;
typedef uint16_t record_elem_type;
typedef uint16_t pointer_type;

typedef char byte;

// PARAM
#define RECORD_ELEM_NUM 4
#define BUFFER_DATA_ENTRY_NUM 4
#define BUFFER_INDEX_ENTRY_NUM 4

#define KEY_SIZE sizeof(key_type)
#define PAGE_NUM_SIZE sizeof(page_num_type)
#define POINTER_SIZE sizeof(pointer_type)
#define RECORD_ELEM_SIZE sizeof(record_elem_type)
#define INDEX_ENTRY_SIZE (KEY_SIZE + PAGE_NUM_SIZE)
#define RECORD_SIZE (RECORD_ELEM_NUM * RECORD_ELEM_SIZE)
#define DATA_ENTRY_SIZE (KEY_SIZE + RECORD_SIZE + POINTER_SIZE)
#define BUFFER_DATA_SIZE (BUFFER_DATA_ENTRY_NUM * DATA_ENTRY_SIZE)
#define BUFFER_INDEX_SIZE (BUFFER_INDEX_ENTRY_NUM * INDEX_ENTRY_SIZE)

#define MAX_ERROR_MESSAGE_LEN 200

// value that indicates that there's no pointer
#define NO_POINTER_VAL 0x0000

typedef enum Flag
{
	UNDEFINED,
	WRONG_PAGE_NUM,
	OK,
	WRONG_KEY,
	WRONG_KEY_ROOT_KEY,
	WRONG_KEY_NULL_KEY,
	REORGANISED,
	BUFFER_NOT_FULL,
	BUFFER_FULL
} Flag;

// TODO: Im assuming that index buffer and data buffer can have different sizes

typedef enum IOMode
{
	READ,
	WRITE,
	UNAVAILABLE
} IOMode;

typedef enum FileType
{
	INDEX,
	DATA
} FileType;

typedef struct Buffer
{
	byte* data; // data it holds (can be indexes, keys, etc.)
	int capacity; // current capacity; = 0 means that the buffer is empty
	int byte_offset; // pointer that points to current byte in the buffer (with each read/write it moves forward)
	int max_capacity; // maximum capacity that this buffer can have; = BUFFER_INDEX_SIZE; = BUFFER_DATA_SIZE
	int dest_page_num; // page number where the buffer will write to/read from
	FILE* file; // opened file from which data is read
	//char file_path[MAX_FILE_PATH_LEN];
	FileType type;
	IOMode io_mode;
	int reads;
	int writes;
} Buffer;

void clear_buffer(Buffer* buffer);

Flag read_page(Buffer* buffer);

Flag read_index_entry(Buffer* buffer, key_type* key, page_num_type* data_page_num);

Flag read_data_entry(Buffer* buffer, key_type* key, record_elem_type record[RECORD_ELEM_NUM], pointer_type* pointer);

void write_page(Buffer* buffer);

// writes to file and clears
void flush_buffer(Buffer* buffer);

Flag write_data_entry(Buffer* buffer, key_type key, record_elem_type record[RECORD_ELEM_NUM], pointer_type pointer);

Flag write_index_entry(Buffer* buffer, key_type key, page_num_type data_page_num);

void init_buffer(Buffer** buffer, FileType buffer_type, IOMode io_mode);

void delete_buffer(Buffer* buffer);

void print_buffer(Buffer* buffer);

void set_buffer_dest_page(Buffer* buffer, int page_num);

void handle_error(char* message);

void set_data_entry_at(Buffer* buffer, int local_entry, key_type key, record_elem_type record[RECORD_ELEM_NUM], pointer_type pointer);

void set_index_entry_at(Buffer* buffer, int local_entry, key_type key, page_num_type data_page_num);

void get_data_entry_at(Buffer* buffer, int local_entry, key_type* key, record_elem_type record[RECORD_ELEM_NUM], pointer_type* pointer);

void get_index_entry_at(Buffer* buffer, int local_entry, key_type* key, page_num_type* data_page_num);

void reset_buffer_stats(Buffer* buffer);