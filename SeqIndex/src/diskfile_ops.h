#pragma once

#include "buffer_ops.h"
#include <math.h>

// NULL_POINTER is the maximum value of the pointer_type (for example -1 for uint16_t is 65535)
#define NULL_POINTER ((pointer_type) -1)
#define NULL_KEY 0

// DELETED_ENTRY_RECORD_ELEM is the maximum value of the record_elem_type
#define DELETED_ENTRY_RECORD_ELEM ((record_elem_type) -1)
#define PAGE_UTILISATION_FACTOR 0.5

// TODO: change this value to 0.2
#define PRIMARY_OVERFLOW_BALANCE_FACTOR 1.0

#define MAX_FORMAT_STR_LEN 30


// minimum value is 3
#define PRINT_FIELD_LEN 5

typedef struct DiskFile
{
	Buffer* buffer;
	int entries_num; // number of entries in the file
	char file_path[MAX_FILE_PATH_LEN];
} DiskFile;

// creates an empty file and keeps it open in WRITE mode
void create_disk_file(DiskFile** disk_file, char* file_path, FileType file_type);

void set_file_io_mode(DiskFile* disk_file, IOMode io_mode, Bool to_clear);

Flag insert_record(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr, key_type new_key, record_elem_type new_record[RECORD_ELEM_NUM]);

void delete_disk_file(DiskFile* disk_file);

void print_file(DiskFile* disk_file);

void init_diskfiles(DiskFile* index_file, DiskFile* primary_file, DiskFile* overflow_file);

void update_data_entry_at(DiskFile* disk_file, int entry, key_type key, record_elem_type record[RECORD_ELEM_NUM], pointer_type pointer);

void init_file_page(DiskFile* data_file, int page_num);

void reorganise(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr);

Flag update_record(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr, key_type key, record_elem_type new_record[RECORD_ELEM_NUM]);

Flag delete_record(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr, key_type key);