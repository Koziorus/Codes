#include "file_operations.h"

#define FORMAT_OPT_LEN 50

void create_file(const char* path)
{
	FILE* new_file;
	// create a file if it doesn't exist and don't erase it if it already exists
	int err = fopen_s(&new_file, path, "r");
	if (err != 0)
	{
		fopen_s(&new_file, path, "w");
	}
	fclose(new_file);
}

void initialize_block_buffer(BlockBuff* block_b, const char* filepath)
{
	strcpy(block_b->assigned_file_path, filepath);
	clear_block_buffer(block_b);
}

void clear_block_buffer(BlockBuff* block_b)
{
	block_b->capacity = FULL_BLOCK_RECORD_NUM;
	block_b->record_num = 0;

	for (int i = 0; i < FULL_BLOCK_RECORD_NUM; i++)
	{
		// NOTE: if max records num is low, then "empty" might be too long of a string to fit in block_b->records[i]
		strcpy(block_b->records[i], "empty"); 
	}
}

void write_record(BlockBuff* block_b, const char* record_content)
{
	// add record to block buffer
	strcpy(block_b->records[block_b->record_num], record_content);
	block_b->record_num++;
	
	// buffer is at capacity
	if (block_b->record_num == block_b->capacity)
	{
		// write buffer block to file
		for (int i = 0; i < block_b->record_num; i++)
		{
			fprintf(block_b->file, "%s\n", block_b->records[i]);
		}

		block_b->disk_writes++;

		clear_block_buffer(block_b);
	}
}

// NOTE: it works even when there are no records in the block buffer
void flush_block_buffer(BlockBuff* block_b)
{
	// write buffer block to file
	for (int i = 0; i < block_b->record_num; i++)
	{
		fprintf(block_b->file, "%s\n", block_b->records[i]);
	}

	block_b->disk_writes++;

	clear_block_buffer(block_b);
}

const char* read_record(BlockBuff* block_b)
{
	if (block_b->record_num == 0)
	{
		block_b->disk_reads++;

		block_b->capacity = 0;
		for (int i = 0; i < FULL_BLOCK_RECORD_NUM; i++)
		{
			char in[MAX_RECORD_SIZE + 1]; // record content with new line
			char* end = fgets(in, MAX_RECORD_SIZE + 1, block_b->file);

			// end of file
			if (end == NULL)
			{
				break;
			}

			if (in[strlen(in) - 1] == '\n')
			{
				in[strlen(in) - 1] = '\0'; //remove new line from the inputted record
			}

			block_b->capacity++;
			block_b->record_num++;
			strcpy(block_b->records[block_b->record_num - 1], in);
		}
	}
		
	block_b->record_num--;

	// this will happen if the block buffer was empty and the file was already read, so there's no record to return to the caller of read_record
	if (block_b->capacity - (block_b->record_num + 1) >= block_b->capacity)
	{
		// reverting disk_read (because it didn't happen)
		block_b->disk_reads--;
		return NULL;
	}

	return block_b->records[block_b->capacity - (block_b->record_num + 1)]; // + 1 to compensate decrementing record_num before, because we can't decremenet it after return;  

	// NOTE: here it might be neccessary to create a copy of the record string to avoid a situation where the record string is in an another buffer and the previous buffer, where the record pointer is from, is cleared -> this is handled by the invoker (tapes.c) and besides, if the return value was NULL then the invoker couldn't capture this with a string (the invoker can do this only with a const char* (not const char[]), but then strcpy onto a pointer requires previous dynamic allocation on the invoker side)  
}

void clear_disk_operation_counts(BlockBuff* block_b)
{
	block_b->disk_reads = 0;
	block_b->disk_writes = 0;
}



// ATTENTION: in the block buffer with array of records: record strings should be copied (malloc / ?strcpy?) to another buffers, not just assigned, because sooner of later the previous buffer, whose the owner of a record string pointer, will be cleared/changed

// NOTE: note the order of block buffer records (how it is read and writeen from and into the file and what does block_b->record_num mean)