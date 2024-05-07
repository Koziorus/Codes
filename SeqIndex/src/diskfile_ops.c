#include "diskfile_ops.h"

// when defined makes the print_file print raw values from file
//#define RAW_PRINT

// TODO: make sure that after an operation on buffer it is cleared (for example init_primary_file_page clears the buffer so that it has 0 capacity)
// TODO: make sure that buffers are in the right io modes before an operation

// creates an empty file and keeps it open in WRITE mode
void create_disk_file(DiskFile** disk_file, char* file_path, FileType file_type)
{
	(*disk_file) = (DiskFile*)malloc(sizeof(DiskFile));
	init_buffer(&((*disk_file)->buffer), file_type, WRITE);
	strcpy_s((*disk_file)->file_path, MAX_FILE_PATH_LEN, file_path);
	(*disk_file)->entries_num = 0;
	fopen_s(&((*disk_file)->buffer->file), file_path, "wb");
}

// requires set destination page; no clear
void update_data_entry_at(DiskFile* disk_file, int local_entry, key_type key, record_elem_type record[RECORD_ELEM_NUM], pointer_type pointer)
{
	set_file_io_mode(disk_file, READ, FALSE);
	read_page(disk_file->buffer);
	set_data_entry_at(disk_file->buffer, local_entry, key, record, pointer);
	set_file_io_mode(disk_file, WRITE, FALSE);
	write_page(disk_file->buffer);
}

// requires set destination page; no clear
void update_index_entry_at(DiskFile* disk_file, int local_entry, key_type key, page_num_type data_page_num)
{
	set_file_io_mode(disk_file, READ, FALSE);
	read_page(disk_file->buffer);
	set_index_entry_at(disk_file->buffer, local_entry, key, data_page_num);
	set_file_io_mode(disk_file, WRITE, FALSE);
	write_page(disk_file->buffer);
}

// fills a file page with empty entries
void init_file_page(DiskFile* disk_file, int page_num)
{
	set_file_io_mode(disk_file, WRITE, TRUE);
	set_buffer_dest_page(disk_file->buffer, page_num);
	int x = fseek(disk_file->buffer->file, page_num * BUFFER_DATA_SIZE, SEEK_SET);

	if (disk_file->buffer->type == DATA)
	{
		// fill with empty entries
		for (int i = 0; i < BUFFER_DATA_ENTRY_NUM; i++)
		{
			record_elem_type null_record[RECORD_ELEM_NUM];
			memset(null_record, 0, RECORD_SIZE);
			write_data_entry(disk_file->buffer, NULL_KEY, null_record, NULL_POINTER);
		}
	}
	else if (disk_file->buffer->type == INDEX)
	{
		for (int i = 0; i < BUFFER_INDEX_ENTRY_NUM; i++)
		{
			write_index_entry(disk_file->buffer, NULL_KEY, 0);
		}
	}

	flush_buffer(disk_file->buffer);
}

void init_diskfiles(DiskFile* index_file, DiskFile* primary_file, DiskFile* overflow_file)
{
	set_file_io_mode(index_file, WRITE, TRUE);
	Buffer* index_buffer = index_file->buffer;
	set_buffer_dest_page(index_buffer, 0);
	init_file_page(index_file, 0);
	key_type key = 1;
	write_index_entry(index_buffer, key, 0);
	flush_buffer(index_buffer);
	index_file->entries_num = 1;

	set_file_io_mode(primary_file, WRITE, TRUE);
	init_file_page(primary_file, 0);
	primary_file->entries_num = 1;
	
	set_buffer_dest_page(primary_file->buffer, 0);
	record_elem_type null_record[RECORD_ELEM_NUM];
	memset(null_record, 0, sizeof(record_elem_type) * RECORD_ELEM_NUM); // zero out the record
	update_data_entry_at(primary_file, 0, key, null_record, NULL_POINTER);

	set_file_io_mode(overflow_file, WRITE, TRUE);
	init_file_page(overflow_file, 0);
	overflow_file->entries_num = 0;
}

// TODO: check if set io mode works when the file doesnt exist

void set_file_io_mode(DiskFile* disk_file, IOMode io_mode, Bool to_clear)
{
	if (to_clear == TRUE)
	{
		clear_buffer(disk_file->buffer);
	}

	fclose(disk_file->buffer->file);

	if (io_mode == READ)
	{
		disk_file->buffer->io_mode = READ;
		fopen_s(&(disk_file->buffer->file), disk_file->file_path, "rb");
	}
	else if (io_mode == WRITE)
	{
		disk_file->buffer->io_mode = WRITE;
		
		if (fopen_s(&(disk_file->buffer->file), disk_file->file_path, "rb+") != 0)
		{
			fopen_s(&(disk_file->buffer->file), disk_file->file_path, "wb"); // if file doesn't exist
		}
	}
}

// sets correct destination page; requires IO mode WRITE
int get_new_local_entry(DiskFile* disk_file)
{
	int last_overflow_entry = disk_file->entries_num - 1, new_overflow_entry = disk_file->entries_num; // entry within file

	int new_overflow_page_num = (new_overflow_entry / BUFFER_DATA_ENTRY_NUM); // page for the new entry
	if ((new_overflow_entry / BUFFER_DATA_ENTRY_NUM) > (last_overflow_entry / BUFFER_DATA_ENTRY_NUM))
	{
		init_file_page(disk_file, new_overflow_page_num);
	}

	set_buffer_dest_page(disk_file->buffer, new_overflow_page_num);
	int overflow_local_entry = new_overflow_entry % BUFFER_DATA_ENTRY_NUM; // entry withing page
	return overflow_local_entry;
}

// requires to set destinaton page
int local_to_global_entry(DiskFile* disk_file, int local_entry)
{
	int global_entry = local_entry + ((disk_file->buffer->type == DATA ? BUFFER_DATA_ENTRY_NUM : BUFFER_INDEX_ENTRY_NUM) * disk_file->buffer->dest_page_num);
	return global_entry;
}

int global_to_local_entry(DiskFile* disk_file, int global_entry)
{
	int local_entry = global_entry % (disk_file->buffer->type == DATA ? BUFFER_DATA_ENTRY_NUM : BUFFER_INDEX_ENTRY_NUM);
	return local_entry;
}

// format of a diskfile filename = vNUMBERNAMEOFFILE (for example v12index_file
void get_new_filename_version(char* old_file_name, char* new_file_name)
{
	char file_main_name[MAX_FILE_PATH_LEN];
	int file_version_number;
	sscanf_s(old_file_name, "%[^0-9]%d", file_main_name, MAX_FILE_PATH_LEN, &file_version_number);
	sprintf_s(new_file_name, MAX_FILE_PATH_LEN, "%s%d", file_main_name, file_version_number + 1);
}

void delete_disk_file(DiskFile* disk_file)
{
	fclose(disk_file->buffer->file);
	delete_buffer(disk_file->buffer);
	free(disk_file);
}

// TODO: dont allocate new pages on the fly (do them only in the beginning and in reorganisation)
// you can find out how many pages are needed based on the parameters

// TODO: check in each function of diskfile_ops if the right page is set

// requires primary file IO mode WRITE
void reorgranisation_insert(DiskFile* index_file, DiskFile* primary_file, DiskFile* overflow_file, key_type key, record_elem_type record[RECORD_ELEM_NUM], int* primary_loc_entries_num, float alpha)
{
	// new page in primary file
	if (((float)*primary_loc_entries_num / BUFFER_DATA_ENTRY_NUM) >= alpha)
	{
		// create a new page in primary file and change destination page
		(*primary_loc_entries_num) = 0;
		int new_primary_dest_page_num = primary_file->buffer->dest_page_num + 1;
		init_file_page(primary_file, new_primary_dest_page_num);
		clear_buffer(primary_file->buffer);
		set_buffer_dest_page(primary_file->buffer, new_primary_dest_page_num);

		// add entry to index file
		set_file_io_mode(index_file, WRITE, TRUE);
		int index_loc_entry = get_new_local_entry(index_file); // will create a new page in index file if neccessary
		update_index_entry_at(index_file, index_loc_entry, key, primary_file->buffer->dest_page_num);
		index_file->entries_num++;
	}

	update_data_entry_at(primary_file, *primary_loc_entries_num, key, record, NULL_POINTER);
	(*primary_loc_entries_num)++;

	primary_file->entries_num++;
}

// NOTE: it might turn out that the code uses to much read_page and write_page which causes the number of reads/writes to disk too big

void reorganise(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr)
{
	// o_... means old files/entries/...
	// n_... means new files/entries/...
	// ..._loc_... means local 
	DiskFile* o_index_file = *index_file_ptr;
	DiskFile* o_primary_file = *primary_file_ptr;
	DiskFile* o_overflow_file = *overflow_file_ptr;

	DiskFile* n_index_file;
	DiskFile* n_primary_file;
	DiskFile* n_overflow_file;

	char n_index_file_name[MAX_FILE_PATH_LEN], n_primary_file_name[MAX_FILE_PATH_LEN], n_overflow_file_name[MAX_FILE_PATH_LEN];
	get_new_filename_version(o_index_file->file_path, n_index_file_name);
	get_new_filename_version(o_primary_file->file_path, n_primary_file_name);
	get_new_filename_version(o_overflow_file->file_path, n_overflow_file_name);

	create_disk_file(&n_index_file, n_index_file_name, INDEX);
	create_disk_file(&n_primary_file, n_primary_file_name, DATA);
	create_disk_file(&n_overflow_file, n_overflow_file_name, DATA);

	init_diskfiles(n_index_file, n_primary_file, n_overflow_file);

	set_file_io_mode(n_index_file, WRITE, TRUE);
	set_file_io_mode(n_primary_file, WRITE, TRUE);
	set_file_io_mode(n_overflow_file, WRITE, TRUE);

	set_buffer_dest_page(n_index_file->buffer, 0);
	set_buffer_dest_page(n_primary_file->buffer, 0);
	// overflow dest page 0

	clear_buffer(o_index_file->buffer);
	clear_buffer(o_primary_file->buffer);
	clear_buffer(o_overflow_file->buffer);

	set_file_io_mode(o_index_file, READ, TRUE);
	set_file_io_mode(o_primary_file, READ, TRUE);
	set_file_io_mode(o_overflow_file, READ, TRUE);

	set_buffer_dest_page(o_primary_file->buffer, 0);

	// after initializating new files the primary file has one entry (root with key = 1) 
	int n_primary_loc_entries_num = 1;

	Buffer* o_index_buffer = o_index_file->buffer;
	Buffer* o_primary_buffer = o_primary_file->buffer;
	Buffer* o_overflow_buffer = o_overflow_file->buffer;

	// go through all index entries
	for (int o_index_entry = 0; o_index_entry < o_index_file->entries_num; o_index_entry++)
	{
		int o_index_page = o_index_entry / BUFFER_INDEX_ENTRY_NUM;
		set_buffer_dest_page(o_index_buffer, o_index_page);
		key_type index_key;
		page_num_type o_data_page_num;
		read_index_entry(o_index_buffer, &index_key, &o_data_page_num);

		clear_buffer(o_primary_buffer);
		set_buffer_dest_page(o_primary_buffer, o_data_page_num);

		// go through a page of primary entries
		for (int o_primary_loc_entry = 0; o_primary_loc_entry < BUFFER_DATA_ENTRY_NUM; o_primary_loc_entry++)
		{
			key_type key;
			record_elem_type record[RECORD_ELEM_NUM];
			pointer_type pointer;

			read_data_entry(o_primary_buffer, &key, record, &pointer);

			// when there are no more entries in this old primary page
			if (key == NULL_KEY)
			{
				break;
			}

			int o_overflow_entry = 0;

			// go through a chain of oveflows
			for (int i = 0; 1; i++)
			{
				if (i != 0)
				{
					int o_overflow_page = o_overflow_entry / BUFFER_DATA_ENTRY_NUM;
					set_buffer_dest_page(o_overflow_buffer, o_overflow_page);
					set_file_io_mode(o_overflow_file, READ, TRUE);
					read_page(o_overflow_buffer);
					int o_overflow_loc_entry = global_to_local_entry(o_overflow_file, o_overflow_entry);
					get_data_entry_at(o_overflow_buffer, o_overflow_loc_entry, &key, record, &pointer);
				}

				//if it's not the root primary entry 
				if (key != 1)
				{
					if (record[0] != DELETED_ENTRY_RECORD_ELEM)
					{
						reorgranisation_insert(n_index_file, n_primary_file, n_overflow_file, key, record, &n_primary_loc_entries_num, PAGE_UTILISATION_FACTOR);
					}
				}

				if (pointer == NULL_POINTER)
				{
					break;
				}
				else
				{
					o_overflow_entry = pointer;
				}
			}
		}
	}

	// adding stats that would be lost
	n_index_file->buffer->reads += o_index_buffer->reads;
	n_primary_file->buffer->reads += o_primary_buffer->reads;
	n_overflow_file->buffer->reads += o_overflow_buffer->reads;
	n_index_file->buffer->writes += o_index_buffer->writes;
	n_primary_file->buffer->writes += o_primary_buffer->writes;
	n_overflow_file->buffer->writes += o_overflow_buffer->writes;

	// delete old disk_files
	delete_disk_file(*index_file_ptr);
	delete_disk_file(*primary_file_ptr);
	delete_disk_file(*overflow_file_ptr);

	// replace old disk_files with new ones
	*index_file_ptr = n_index_file;
	*primary_file_ptr = n_primary_file;
	*overflow_file_ptr = n_overflow_file;
}

// double pointer of diskfiles is for a situaion when there is reorganisation inside insert_record() and the addresses of DiskFile's change
Flag insert_record(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr, key_type new_key, record_elem_type new_record[RECORD_ELEM_NUM])
{
	DiskFile* index_file = *index_file_ptr;
	DiskFile* primary_file = *primary_file_ptr;
	DiskFile* overflow_file = *overflow_file_ptr;

	set_file_io_mode(index_file, READ, TRUE);
	key_type prev_file_key = 0, next_file_key = 0;
	page_num_type next_data_page_num = 0, prev_data_page_num = 0;
	
	// searching for a data_page_num in index file
	for (int index_entry = 0; next_file_key < new_key && index_entry < index_file->entries_num; index_entry++)
	{
		prev_file_key = next_file_key;
		prev_data_page_num = next_data_page_num;
		set_buffer_dest_page(index_file->buffer, index_entry / BUFFER_INDEX_ENTRY_NUM);
		read_index_entry(index_file->buffer, &next_file_key, &next_data_page_num);

		if (index_entry == 0)
		{
			prev_file_key = next_file_key;
			prev_data_page_num = next_data_page_num;
		}
	}

	key_type file_key; // the searched key
	page_num_type data_page_num; // the searched primary page number

	if (next_file_key > new_key)
	{
		file_key = prev_file_key;
		data_page_num = prev_data_page_num;
	}
	else
	{
		file_key = next_file_key;
		data_page_num = next_data_page_num;
	}
	
	set_file_io_mode(primary_file, READ, TRUE);
	set_buffer_dest_page(primary_file->buffer, data_page_num);
	
	pointer_type pointer;

	int local_primary_entry = 0;

	//searching for the right place in data file page
	for (int i = 0; i == 0 || (file_key < new_key && local_primary_entry < BUFFER_DATA_ENTRY_NUM && file_key != 0); i++)
	{
		record_elem_type dummy_record[RECORD_ELEM_NUM];
		read_data_entry(primary_file->buffer, &file_key, dummy_record, &pointer);

		local_primary_entry++;
	}
	local_primary_entry--;

	// is the primary entry from which new overflow entry is created
	int smaller_local_entry = local_primary_entry;
	if (file_key < new_key)
	{
		smaller_local_entry = local_primary_entry;
	}
	else
	{
		smaller_local_entry = local_primary_entry - 1;
	}

	if (file_key == new_key)
	{
		//char error_message[MAX_ERROR_MESSAGE_LEN];
		//sprintf_s(error_message, MAX_ERROR_MESSAGE_LEN, "Record with key %d already exists!\n", new_key);
		//handle_error(error_message);
		return WRONG_KEY;
	}

	read_page(primary_file->buffer);
	key_type last_entry_key;
	record_elem_type dummy_record[RECORD_ELEM_NUM];
	pointer_type dummy_pointer;
	get_data_entry_at(primary_file->buffer, BUFFER_DATA_ENTRY_NUM - 1, &last_entry_key, dummy_record, &dummy_pointer);
		
	// data page full
	if (last_entry_key != NULL_KEY)
	{
		// primary_entry points here at entry that has a smaller key than new_key
		
		read_page(primary_file->buffer);
		key_type primary_key;
		record_elem_type primary_record[RECORD_ELEM_NUM];
		pointer_type primary_pointer;
		get_data_entry_at(primary_file->buffer, smaller_local_entry, &primary_key, primary_record, &primary_pointer);

		if (primary_pointer == NULL_POINTER) //  entry at pointer of primary entry IS empty
		{
			pointer_type new_pointer = overflow_file->entries_num;
			set_file_io_mode(primary_file, WRITE, FALSE);
			set_data_entry_at(primary_file->buffer, smaller_local_entry, primary_key, primary_record, new_pointer);
			flush_buffer(primary_file->buffer);

			int new_local_entry = get_new_local_entry(overflow_file);
			update_data_entry_at(overflow_file, new_local_entry, new_key, new_record, NULL_POINTER);
		}
		else // entry at pointer of primary entry IS NOT empty
		{
			set_file_io_mode(overflow_file, READ, TRUE);
			
			key_type prev_key, next_key;
			record_elem_type prev_record[RECORD_ELEM_NUM], next_record[RECORD_ELEM_NUM];
			pointer_type prev_pointer, next_pointer;

			prev_pointer = primary_pointer;

			int prev_overflow_page, prev_local_entry;

			for(int i = 0; 1; i++)
			{
				int next_overflow_page = prev_pointer / BUFFER_DATA_ENTRY_NUM;
				set_buffer_dest_page(overflow_file->buffer, next_overflow_page);
				read_page(overflow_file->buffer);
				int next_local_entry = prev_pointer % BUFFER_DATA_ENTRY_NUM;

				get_data_entry_at(overflow_file->buffer, next_local_entry, &next_key, next_record, &next_pointer);

				if (next_key == new_key)
				{
					//char error_message[MAX_ERROR_MESSAGE_LEN];
					//sprintf_s(error_message, MAX_ERROR_MESSAGE_LEN, "Record with key %d already exists!\n", new_key);
					//handle_error(error_message);
					return WRONG_KEY; //
				}

				// new entry is written at the beginning of the overflow chain
				if (i == 0 && next_key > new_key)
				{
					set_file_io_mode(overflow_file, WRITE, TRUE);
					int new_local_entry = get_new_local_entry(overflow_file);
					int new_entry = local_to_global_entry(overflow_file, new_local_entry);
					update_data_entry_at(overflow_file, new_local_entry, new_key, new_record, prev_pointer);

					set_file_io_mode(primary_file, WRITE, TRUE);
					update_data_entry_at(primary_file, smaller_local_entry, primary_key, primary_record, new_entry);

					break;
				}

				if (next_key > new_key) // new entry is written in the middle of the overflow chain
				{
					set_file_io_mode(overflow_file, WRITE, TRUE);
					int new_local_entry = get_new_local_entry(overflow_file);
					int new_entry = local_to_global_entry(overflow_file, new_local_entry);
					update_data_entry_at(overflow_file, new_local_entry, new_key, new_record, prev_pointer);

					set_buffer_dest_page(overflow_file->buffer, prev_overflow_page);
					update_data_entry_at(overflow_file, prev_local_entry, prev_key, prev_record, new_entry);

					break;
				}
				else if (next_pointer == NULL_POINTER) // key < new_key or NULL_POINTER
				{
					// new entry is written at the end of overflow chain

					set_file_io_mode(overflow_file, WRITE, TRUE);

					pointer_type pointer = overflow_file->entries_num;
					update_data_entry_at(overflow_file, next_local_entry, next_key, next_record, pointer);

					int new_local_entry = get_new_local_entry(overflow_file);
					update_data_entry_at(overflow_file, new_local_entry, new_key, new_record, NULL_POINTER);

					break;
				}
				else // key < new_key
				{
					prev_pointer = next_pointer;
				}

				prev_key = next_key;
				memcpy(prev_record, next_record, RECORD_SIZE);
				prev_pointer = next_pointer;
				prev_overflow_page = next_overflow_page;
				prev_local_entry = next_local_entry;
			}
		}

		overflow_file->entries_num++;
	}
	else //data page not full
	{
		// primary_entry points here at entry that has a greater key than new_key
		read_page(primary_file->buffer);
		set_file_io_mode(primary_file, WRITE, FALSE);
		// moves the content of the buffer to make space for the new record
		memmove(&(primary_file->buffer->data[((long long)local_primary_entry + 1) * DATA_ENTRY_SIZE]), &(primary_file->buffer->data[local_primary_entry * DATA_ENTRY_SIZE]), DATA_ENTRY_SIZE * (BUFFER_DATA_ENTRY_NUM - (long long)local_primary_entry - 1));
		set_data_entry_at(primary_file->buffer, local_primary_entry, new_key, new_record, NULL_POINTER);

		flush_buffer(primary_file->buffer);
		primary_file->entries_num++;
	}

	if ((*overflow_file_ptr)->entries_num / (float)(*primary_file_ptr)->entries_num >= PRIMARY_OVERFLOW_BALANCE_FACTOR)
	{
		//printf("Reorganising...\n");
		reorganise(index_file_ptr, primary_file_ptr, overflow_file_ptr);
		return REORGANISED;
	}



	return OK; // successful
}

Flag update_record(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr, key_type key, record_elem_type new_record[RECORD_ELEM_NUM])
{
	if (key == 1)
	{
		return WRONG_KEY_ROOT_KEY; // root record can't be changed
	}
	else if (key == NULL_KEY)
	{
		return WRONG_KEY_NULL_KEY;
	}

	DiskFile* index_file = *index_file_ptr;
	DiskFile* primary_file = *primary_file_ptr;
	DiskFile* overflow_file = *overflow_file_ptr;

	set_file_io_mode(index_file, READ, TRUE);
	key_type prev_file_key = 0, next_file_key = 0;
	page_num_type next_data_page_num = 0, prev_data_page_num = 0;

	// searching for a data_page_num in index file
	for (int index_entry = 0; next_file_key < key && index_entry < index_file->entries_num; index_entry++)
	{
		prev_file_key = next_file_key;
		prev_data_page_num = next_data_page_num;
		set_buffer_dest_page(index_file->buffer, index_entry / BUFFER_INDEX_ENTRY_NUM);
		read_index_entry(index_file->buffer, &next_file_key, &next_data_page_num);

		if (index_entry == 0)
		{
			prev_file_key = next_file_key;
			prev_data_page_num = next_data_page_num;
		}
	}

	key_type file_key; // the searched key
	page_num_type data_page_num; // the searched primary page number

	if (next_file_key > key)
	{
		file_key = prev_file_key;
		data_page_num = prev_data_page_num;
	}
	else
	{
		file_key = next_file_key;
		data_page_num = next_data_page_num;
	}

	set_file_io_mode(primary_file, READ, TRUE);
	set_buffer_dest_page(primary_file->buffer, data_page_num);

	pointer_type next_pointer = 0, prev_pointer = 0;

	int local_primary_entry = 0;

	//searching for the right place in data file page
	for (int i = 0; i == 0 || (file_key < key && local_primary_entry < BUFFER_DATA_ENTRY_NUM && file_key != 0); i++)
	{
		prev_pointer = next_pointer;
		record_elem_type dummy_record[RECORD_ELEM_NUM];
		read_data_entry(primary_file->buffer, &file_key, dummy_record, &next_pointer);

		if (i == 0)
		{
			prev_pointer = next_pointer;
		}

		local_primary_entry++;
	}

	local_primary_entry--;

	int smaller_local_entry = local_primary_entry; // is the primary entry from which overflow entries will be searched
	if (file_key < key)
	{
		smaller_local_entry = local_primary_entry;
	}
	else
	{
		smaller_local_entry = local_primary_entry - 1;
	}

	// update record in primary file
	if (file_key == key)
	{
		update_data_entry_at(primary_file, local_primary_entry, key, new_record, next_pointer);

		return OK;
	}

	// primary_entry points here at entry that has a smaller key than new_key

	read_page(primary_file->buffer);
	key_type primary_key;
	record_elem_type primary_record[RECORD_ELEM_NUM];
	pointer_type primary_pointer;
	get_data_entry_at(primary_file->buffer, smaller_local_entry, &primary_key, primary_record, &primary_pointer);

	if (primary_pointer == NULL_POINTER) //  entry at pointer of primary entry IS empty
	{
		return WRONG_KEY;
	}
	else // entry at pointer of primary entry IS NOT empty
	{
		set_file_io_mode(overflow_file, READ, TRUE);

		key_type prev_key, next_key;
		record_elem_type prev_record[RECORD_ELEM_NUM], next_record[RECORD_ELEM_NUM];
		pointer_type prev_pointer, next_pointer;

		prev_pointer = primary_pointer;

		int prev_overflow_page, prev_local_entry;

		for (int i = 0; 1; i++)
		{
			int next_overflow_page = prev_pointer / BUFFER_DATA_ENTRY_NUM;
			set_buffer_dest_page(overflow_file->buffer, next_overflow_page);
			read_page(overflow_file->buffer);
			int next_local_entry = prev_pointer % BUFFER_DATA_ENTRY_NUM;

			get_data_entry_at(overflow_file->buffer, next_local_entry, &next_key, next_record, &next_pointer);

			if (next_key == key)
			{
				update_data_entry_at(overflow_file, next_local_entry, next_key, new_record, next_pointer);
				return OK;
			}

			if (next_key > key || next_pointer == NULL_POINTER) // new entry is written in the middle of the overflow chain
			{
				return WRONG_KEY;
			}
			else // next_key < key
			{
				prev_pointer = next_pointer;
			}

			prev_key = next_key;
			memcpy(prev_record, next_record, RECORD_SIZE);
			prev_pointer = next_pointer;
			prev_overflow_page = next_overflow_page;
			prev_local_entry = next_local_entry;
		}
	}

	return OK;
}

Flag delete_record(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr, key_type key)
{
	if (key == 1)
	{
		return WRONG_KEY_ROOT_KEY; // root record can't be deleted
	}
	else if (key == NULL_KEY)
	{
		return WRONG_KEY_NULL_KEY;
	}

	DiskFile* index_file = *index_file_ptr;
	DiskFile* primary_file = *primary_file_ptr;
	DiskFile* overflow_file = *overflow_file_ptr;

	set_file_io_mode(index_file, READ, TRUE);
	key_type prev_file_key = 0, next_file_key = 0;
	page_num_type next_data_page_num = 0, prev_data_page_num = 0;

	// searching for a data_page_num in index file
	for (int index_entry = 0; next_file_key < key && index_entry < index_file->entries_num; index_entry++)
	{
		prev_file_key = next_file_key;
		prev_data_page_num = next_data_page_num;
		set_buffer_dest_page(index_file->buffer, index_entry / BUFFER_INDEX_ENTRY_NUM);
		read_index_entry(index_file->buffer, &next_file_key, &next_data_page_num);

		if (index_entry == 0)
		{
			prev_file_key = next_file_key;
			prev_data_page_num = next_data_page_num;
		}
	}

	key_type file_key; // the searched key
	page_num_type data_page_num; // the searched primary page number

	if (next_file_key > key)
	{
		file_key = prev_file_key;
		data_page_num = prev_data_page_num;
	}
	else
	{
		file_key = next_file_key;
		data_page_num = next_data_page_num;
	}

	set_file_io_mode(primary_file, READ, TRUE);
	set_buffer_dest_page(primary_file->buffer, data_page_num);

	pointer_type next_pointer = 0;
	record_elem_type record[RECORD_ELEM_NUM];

	int local_primary_entry = 0;

	//searching for the right place in data file page
	
	for(int i = 0; i == 0 || (file_key < key && local_primary_entry < BUFFER_DATA_ENTRY_NUM && file_key != 0); i++)
	{
		read_data_entry(primary_file->buffer, &file_key, record, &next_pointer);

		local_primary_entry++;
	}
	local_primary_entry--;

	int smaller_local_entry = local_primary_entry; // is the primary entry from which overflow entries will be searched
	if (file_key < key)
	{
		smaller_local_entry = local_primary_entry;
	}
	else
	{
		smaller_local_entry = local_primary_entry - 1;
	}

	// update record in primary file
	if (file_key == key)
	{
		record_elem_type marked_record[RECORD_ELEM_NUM];
		for (int i = 0; i < RECORD_ELEM_NUM; i++)
		{
			marked_record[i] = DELETED_ENTRY_RECORD_ELEM;
		}

		update_data_entry_at(primary_file, local_primary_entry, file_key, marked_record, next_pointer);

		return OK;
	}

	// primary_entry points here at entry that has a smaller key than new_key

	read_page(primary_file->buffer);
	key_type primary_key;
	record_elem_type primary_record[RECORD_ELEM_NUM];
	pointer_type primary_pointer;
	get_data_entry_at(primary_file->buffer, smaller_local_entry, &primary_key, primary_record, &primary_pointer);

	if (primary_pointer == NULL_POINTER) //  entry at pointer of primary entry IS empty
	{
		return WRONG_KEY;
	}
	else // entry at pointer of primary entry IS NOT empty
	{
		set_file_io_mode(overflow_file, READ, TRUE);

		key_type prev_key, next_key;
		record_elem_type prev_record[RECORD_ELEM_NUM], next_record[RECORD_ELEM_NUM];
		pointer_type prev_pointer, next_pointer;

		prev_pointer = primary_pointer;

		int prev_overflow_page, prev_local_entry;

		for (int i = 0; 1; i++)
		{
			int next_overflow_page = prev_pointer / BUFFER_DATA_ENTRY_NUM;
			set_buffer_dest_page(overflow_file->buffer, next_overflow_page);
			read_page(overflow_file->buffer);
			int next_local_entry = prev_pointer % BUFFER_DATA_ENTRY_NUM;

			get_data_entry_at(overflow_file->buffer, next_local_entry, &next_key, next_record, &next_pointer);

			if (next_key == key)
			{
				record_elem_type marked_record[RECORD_ELEM_NUM];
				for (int i = 0; i < RECORD_ELEM_NUM; i++)
				{
					marked_record[i] = DELETED_ENTRY_RECORD_ELEM;
				}

				update_data_entry_at(overflow_file, next_local_entry, next_key, marked_record, next_pointer);
				return OK;
			}

			if (next_key > key || next_pointer == NULL_POINTER) // new entry is written in the middle of the overflow chain
			{
				return WRONG_KEY;
			}
			else // next_key < key
			{
				prev_pointer = next_pointer;
			}

			prev_key = next_key;
			memcpy(prev_record, next_record, RECORD_SIZE);
			prev_pointer = next_pointer;
			prev_overflow_page = next_overflow_page;
			prev_local_entry = next_local_entry;
		}
	}

	return OK;
}

void print_file(DiskFile* disk_file)
{
	set_file_io_mode(disk_file, READ, TRUE);
	Buffer* buffer = disk_file->buffer;

	if (buffer->type == INDEX)
	{
		printf("File (INDEX) \"%s\":\n", disk_file->file_path);
		printf("Page");
		
		for (int i = 0; i < PRINT_FIELD_LEN - 4; i++)
		{
			printf(" ");
		}

		printf(" | Key");

		for (int i = 0; i < PRINT_FIELD_LEN - 3; i++)
		{
			printf(" ");
		}

		printf(" | Data Page");

		for (int i = 0; i < PRINT_FIELD_LEN - 9; i++)
		{
			printf(" ");
		}

		printf("\n");
		
		for (int i = 0; i < (PRINT_FIELD_LEN + 3) * (1 + 1) - 3 + (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN + 3 : 4 + 3); i++)
		{
			printf("=");
		}

		printf("\n");

		Flag read_flag = OK;
		for (int page_num = 0; read_flag != WRONG_PAGE_NUM; page_num++)
		{
			set_buffer_dest_page(buffer, page_num);

			for (int i = 0; buffer->capacity != 0 || i == 0; i++)
			{
				key_type key;
				page_num_type data_page_num;
				read_flag = read_index_entry(buffer, &key, &data_page_num);
				if (read_flag == WRONG_PAGE_NUM)
				{
					break;
				}

				if (i == 0)
				{
					char format_str[MAX_FORMAT_STR_LEN];
					sprintf_s(format_str, MAX_FORMAT_STR_LEN,"%%%dd", (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN : 4));
					printf(format_str, page_num);
				}
				else
				{
					for (int i = 0; i < (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN : 4); i++)
					{
						printf(" ");
					}
				}

#ifdef RAW_PRINT
				printf(" | %5d | %5d\n", key, data_page_num);
#else
				if (key == NULL_KEY)
				{
					printf(" | ");
					for (int i = 0; i < PRINT_FIELD_LEN; i++)
					{
						printf("-");
					}

					printf(" | ");

					for (int i = 0; i < PRINT_FIELD_LEN - 1; i++)
					{
						printf(" ");
					}

					printf("-\n");
				}
				else
				{
					char format_str[MAX_FORMAT_STR_LEN];
					sprintf_s(format_str, MAX_FORMAT_STR_LEN, " | %%%dd | %%%dd\n", PRINT_FIELD_LEN, PRINT_FIELD_LEN);
					printf(format_str, key, data_page_num);
				}
#endif
			}

			if (read_flag != WRONG_PAGE_NUM)
			{
				for (int i = 0; i < (PRINT_FIELD_LEN + 3) * (1 + 1) - 3 + (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN + 3 : 4 + 3); i++)
				{
					printf("=");
				}

				printf("\n");
			}
		}

		printf("\n");
	}
	else if (buffer->type == DATA)
	{
		printf("File (DATA) \"%s\":\n", disk_file->file_path);
		printf("Page");

		for (int i = 0; i < PRINT_FIELD_LEN - 4; i++)
		{
			printf(" ");
		}

		printf(" | Key");

		for (int i = 0; i < PRINT_FIELD_LEN - 3; i++)
		{
			printf(" ");
		}

		printf(" | ");

		for (int i = 0; i < RECORD_ELEM_NUM; i++)
		{
			if (i != 0)
			{
				printf(" | ");
			}

			char format_str[MAX_FORMAT_STR_LEN];
			sprintf_s(format_str, MAX_FORMAT_STR_LEN, "r_%%-%dd", PRINT_FIELD_LEN - 2);
			printf(format_str, i);
		}

		printf(" | ");
		printf("Ptr\n");

		for (int i = 0; i < (PRINT_FIELD_LEN + 3) * (RECORD_ELEM_NUM + 1 + 1) - 3 + (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN + 3 : 4 + 3); i++)
		{
			printf("=");
		}

		printf("\n");

		Flag read_flag = OK;
		for (int page_num = 0; read_flag != WRONG_PAGE_NUM; page_num++)
		{
			// for each entry in buffer
			set_buffer_dest_page(buffer, page_num);
			for (int i = 0; buffer->capacity != 0 || i == 0; i++)
			{
				key_type key;
				record_elem_type record[RECORD_ELEM_NUM];
				pointer_type pointer;
				read_flag = read_data_entry(buffer, &key, record, &pointer);
				if (read_flag == WRONG_PAGE_NUM)
				{
					break;
				}

				if (i == 0)
				{
					char format_str[MAX_FORMAT_STR_LEN];
					sprintf_s(format_str, MAX_FORMAT_STR_LEN, "%%%dd", (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN : 4));
					printf(format_str, page_num);
				}
				else
				{
					for (int i = 0; i < (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN : 4); i++)
					{
						printf(" ");
					}
				}

#ifdef RAW_PRINT
				char format_str[MAX_FORMAT_STR_LEN];
				sprintf_s(format_str, MAX_FORMAT_STR_LEN, " | %%%dd | ", PRINT_FIELD_LEN);
				printf(format_str, key);

				for (int i = 0; i < RECORD_ELEM_NUM; i++)
				{
					if (i != 0)
					{
						printf(" | ");
					}

					char format_str[MAX_FORMAT_STR_LEN];
					sprintf_s(format_str, MAX_FORMAT_STR_LEN, "%%%dd", PRINT_FIELD_LEN);
					printf(format_str, record[i]);
				}

				printf(" | ");
#else

				if (key == NULL_KEY)
				{
					printf(" | ");
					
					for (int i = 0; i < PRINT_FIELD_LEN; i++)
					{
						printf("-");
					}

					printf(" | ");

					for (int i = 0; i < RECORD_ELEM_NUM; i++)
					{
						if (i != 0)
						{
							printf(" | ");
						}

						for (int i = 0; i < PRINT_FIELD_LEN; i++)
						{
							printf("-");
						}
					}

					printf(" | ");
				}
				else
				{
					char format_str[MAX_FORMAT_STR_LEN];
					sprintf_s(format_str, MAX_FORMAT_STR_LEN, " | %%%dd | ", PRINT_FIELD_LEN);
					printf(format_str, key);

					for (int i = 0; i < RECORD_ELEM_NUM; i++)
					{
						if (i != 0)
						{
							printf(" | ");
						}

						if (record[i] == DELETED_ENTRY_RECORD_ELEM)
						{
							for (int i = 0; i < PRINT_FIELD_LEN - 3; i++)
							{
								printf(" ");
							}

							printf("DEL");
						}
						else
						{
							char format_str[MAX_FORMAT_STR_LEN];
							sprintf_s(format_str, MAX_FORMAT_STR_LEN, "%%%dd", PRINT_FIELD_LEN);

							printf(format_str, record[i]);
						}
					}

					printf(" | ");
				}
#endif

#ifdef RAW_PRINT
				char format_str[MAX_FORMAT_STR_LEN];
				sprintf_s(format_str, MAX_FORMAT_STR_LEN, "%%%dd", PRINT_FIELD_LEN);
				printf(format_str, pointer);
#else
				if (pointer == (pointer_type)-1)
				{
					for (int i = 0; i < PRINT_FIELD_LEN - 1; i++)
					{
						printf(" ");
					}

					printf("-");
				}
				else
				{
					char format_str[MAX_FORMAT_STR_LEN];
					sprintf_s(format_str, MAX_FORMAT_STR_LEN, "%%%dd", PRINT_FIELD_LEN);
					printf(format_str, pointer);
				}
#endif

				printf("\n");
			}

			if (read_flag != WRONG_PAGE_NUM)
			{
				for (int i = 0; i < (PRINT_FIELD_LEN + 3) * (RECORD_ELEM_NUM + 1 + 1) - 3 + (PRINT_FIELD_LEN >= 4 ? PRINT_FIELD_LEN + 3 : 4 + 3); i++)
				{
					printf("=");
				}

				printf("\n");
			}
		}

		printf("\n");
	}

	clear_buffer(buffer);
}

