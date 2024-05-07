#include "buffer_ops.h"

void handle_error(char* message)
{
	//system("color 0C");
	//printf("\n%s\n", message);
	//__debugbreak();
}

void clear_buffer(Buffer* buffer)
{
	buffer->capacity = 0;
	buffer->byte_offset = 0;
}

Flag read_page(Buffer* buffer)
{
	//going to the correct page
	fseek(buffer->file, buffer->dest_page_num * (buffer->type == INDEX ? BUFFER_INDEX_SIZE : BUFFER_DATA_SIZE), SEEK_SET);
	int read_bytes_num = fread_s(buffer->data, buffer->max_capacity, ONE_BYTE_SIZE, buffer->max_capacity, buffer->file);
	
	if (read_bytes_num == 0)
	{
		return WRONG_PAGE_NUM;
	}

	buffer->capacity = read_bytes_num;
	buffer->byte_offset = 0;

	buffer->reads++;

	return OK;
}

Flag read_index_entry(Buffer* buffer, key_type* key, page_num_type* data_page_num)
{
	Flag flag = OK;
	if (buffer->capacity == 0)
	{
		flag = read_page(buffer);
		if (flag != OK)
		{
			return flag;
		}
	}

	//read key
	memcpy(key, &(buffer->data[buffer->byte_offset]), KEY_SIZE);
	buffer->byte_offset += KEY_SIZE;
	//read page number
	memcpy(data_page_num, &(buffer->data[buffer->byte_offset]), PAGE_NUM_SIZE);
	buffer->byte_offset += PAGE_NUM_SIZE;

	if (buffer->byte_offset >= buffer->capacity)
	{
		clear_buffer(buffer);
	}

	return flag;
}

Flag read_data_entry(Buffer* buffer, key_type* key, record_elem_type record[RECORD_ELEM_NUM], pointer_type* pointer)
{
	Flag flag = OK;
	if (buffer->capacity == 0)
	{
		flag = read_page(buffer);
	}

	//read key
	memcpy(key, &(buffer->data[buffer->byte_offset]), KEY_SIZE);
	buffer->byte_offset += KEY_SIZE;
	//read record (its elements)
	memcpy(record, &(buffer->data[buffer->byte_offset]), RECORD_ELEM_NUM * RECORD_ELEM_SIZE);
	buffer->byte_offset += RECORD_ELEM_NUM * RECORD_ELEM_SIZE;
	//read pointer
	memcpy(pointer, &(buffer->data[buffer->byte_offset]), POINTER_SIZE);
	buffer->byte_offset += POINTER_SIZE;

	if (buffer->byte_offset >= buffer->capacity)
	{
		clear_buffer(buffer);
	}

	return flag;
}

void write_page(Buffer* buffer)
{
	fseek(buffer->file, buffer->dest_page_num * (buffer->type == INDEX ? BUFFER_INDEX_SIZE : BUFFER_DATA_SIZE), SEEK_SET);
	fwrite(buffer->data, buffer->capacity, 1, buffer->file);
	buffer->writes++;
}

// writes to file and clears
void flush_buffer(Buffer* buffer)
{
	if (buffer->capacity > 0)
	{
		write_page(buffer);
	}

	clear_buffer(buffer);
}

Flag write_data_entry(Buffer* buffer, key_type key, record_elem_type record[RECORD_ELEM_NUM], pointer_type pointer)
{
	memcpy(&(buffer->data[buffer->byte_offset]), &key, KEY_SIZE);
	buffer->byte_offset += KEY_SIZE;
	memcpy(&(buffer->data[buffer->byte_offset]), record, RECORD_ELEM_SIZE * RECORD_ELEM_NUM);
	buffer->byte_offset += RECORD_ELEM_NUM * RECORD_ELEM_SIZE;
	memcpy(&(buffer->data[buffer->byte_offset]), &pointer, POINTER_SIZE);
	buffer->byte_offset += POINTER_SIZE;

	buffer->capacity = buffer->byte_offset;

	Flag flag = BUFFER_NOT_FULL;
	if (buffer->byte_offset >= buffer->max_capacity)
	{
		flush_buffer(buffer);
		flag = BUFFER_FULL;
	}

	return flag;
}

Flag write_index_entry(Buffer* buffer, key_type key, page_num_type data_page_num)
{
	memcpy(&(buffer->data[buffer->byte_offset]), &key, KEY_SIZE);
	buffer->byte_offset += KEY_SIZE;
	memcpy(&(buffer->data[buffer->byte_offset]), &data_page_num, PAGE_NUM_SIZE);
	buffer->byte_offset += PAGE_NUM_SIZE;

	buffer->capacity = buffer->byte_offset;

	Flag flag = BUFFER_NOT_FULL;
	if (buffer->byte_offset >= buffer->max_capacity)
	{
		flush_buffer(buffer);
		flag = BUFFER_FULL;
	}

	return flag;
}

void reset_buffer_stats(Buffer* buffer)
{
	buffer->reads = 0;
	buffer->writes = 0;
}

void init_buffer(Buffer** buffer, FileType buffer_type, IOMode io_mode)
{
	(*buffer) = (Buffer*)malloc(sizeof(Buffer));
	(*buffer)->byte_offset = 0;
	(*buffer)->capacity = 0;
	(*buffer)->max_capacity = (buffer_type == INDEX ? BUFFER_INDEX_SIZE : BUFFER_DATA_SIZE);
	(*buffer)->io_mode = io_mode;
	(*buffer)->data = (char*)malloc((*buffer)->max_capacity);
	(*buffer)->type = buffer_type;
	(*buffer)->dest_page_num = 0;
	(*buffer)->file = NULL;
	(*buffer)->reads = 0;
	(*buffer)->writes = 0;
}

void delete_buffer(Buffer* buffer)
{
	free(buffer->data);
	free(buffer);
}

void print_buffer(Buffer* buffer)
{
	if (buffer->type == INDEX)
	{
		printf("Buffer:\n");
		printf("Key   | Data Page Number\n-------------\n");

		key_type key;
		page_num_type data_page_num;
		int offset = 0;
		while (offset < buffer->byte_offset)
		{
			memcpy(&key, &(buffer->data[offset]), KEY_SIZE);
			offset += KEY_SIZE;
			//read page number
			memcpy(&data_page_num, &(buffer->data[offset]), PAGE_NUM_SIZE);
			offset += PAGE_NUM_SIZE;

			printf("%05d | %05d\n", key, data_page_num);
		}
		printf("\n");
	}
	else if(buffer->type == DATA)
	{
		printf("unimplemented\n");
	}
}

// TODO: make sure that set_/get_ calls have the local entries as arguments

void set_data_entry_at(Buffer* buffer, int local_entry, key_type key, record_elem_type record[RECORD_ELEM_NUM], pointer_type pointer)
{
	int offset = local_entry * DATA_ENTRY_SIZE;
	memcpy(&(buffer->data[offset]), &key, KEY_SIZE);
	offset += KEY_SIZE;
	memcpy(&(buffer->data[offset]), record, RECORD_SIZE);
	offset += RECORD_SIZE;
	memcpy(&(buffer->data[offset]), &pointer, POINTER_SIZE);
}

void set_index_entry_at(Buffer* buffer, int local_entry, key_type key, page_num_type data_page_num)
{
	int offset = local_entry * INDEX_ENTRY_SIZE;
	memcpy(&(buffer->data[offset]), &key, KEY_SIZE);
	offset += KEY_SIZE;
	memcpy(&(buffer->data[offset]), &data_page_num, PAGE_NUM_SIZE);
}

void get_data_entry_at(Buffer* buffer, int local_entry, key_type* key, record_elem_type record[RECORD_ELEM_NUM], pointer_type* pointer)
{
	int offset = local_entry * DATA_ENTRY_SIZE;
	memcpy(key, &(buffer->data[offset]), KEY_SIZE);
	offset += KEY_SIZE;
	memcpy(record, &(buffer->data[offset]), RECORD_SIZE);
	offset += RECORD_SIZE;
	memcpy(pointer, &(buffer->data[offset]), POINTER_SIZE);
}

void get_index_entry_at(Buffer* buffer, int local_entry, key_type* key, page_num_type* data_page_num)
{
	int offset = local_entry * INDEX_ENTRY_SIZE;
	memcpy(key, &(buffer->data[offset]), KEY_SIZE);
	offset += KEY_SIZE;
	memcpy(data_page_num, &(buffer->data[offset]), PAGE_NUM_SIZE);
}

void set_buffer_dest_page(Buffer* buffer, int page_num)
{
	buffer->dest_page_num = page_num;
}