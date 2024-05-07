#include "buffer_ops.h"
#include "diskfile_ops.h"

#define COMMAND_MAX_LEN 200
//#define MAX_KEY_VAL ((key_type)-1)
#define MAX_KEY_VAL 10000
//#define MAX_RECORD_ELEM_VAL ((record_elem_type)-1)
#define MAX_RECORD_ELEM_VAL 1000

int reads = 0, writes = 0;

// TODO: print warning when trying to delete an entry that was deleted

// TODO: remember that in reorganisation the previous buffers have some stats that should or should not be transfered/added to the new buffers

void run_test_file(char* file_name, DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr)
{
	FILE* test_file;
	if (fopen_s(&test_file, file_name, "r") != 0)
	{
		printf("Could not open the file file_name: \"%s\"\n", file_name);
		return;
	}

	// Inserting:
	// I R
	// I G key r_0 r_1 ...
	// R - random
	// G - given

	char command[COMMAND_MAX_LEN];

	while (fscanf_s(test_file, "%s", command, COMMAND_MAX_LEN) > 0)
	{
		if (strcmp(command, "I") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			char option;
			fscanf_s(test_file, " %c", &option, 1);

			key_type key;
			record_elem_type record[RECORD_ELEM_NUM];

			if (option == 'R')
			{
				for (int i = 0; i < RECORD_ELEM_NUM; i++)
				{
					int random_record_elem = (rand() % (MAX_RECORD_ELEM_VAL - 1));
					record[i] = random_record_elem;
				}

				int random_key = (rand() % (MAX_KEY_VAL - 1)) + 2; // from 2 to max
				key = random_key;

				while (insert_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, key, record) == WRONG_KEY)
				{
					int random_key = (rand() % (MAX_KEY_VAL - 1)) + 2; // from 2 to max
					key = random_key;
				}
			}
			else if (option == 'G')
			{
				int key_in;
				fscanf_s(test_file, "%d", &key_in);
				key = key_in;
				for (int i = 0; i < RECORD_ELEM_NUM; i++)
				{
					int record_elem_in;
					fscanf_s(test_file, "%d", &record_elem_in);
					record[i] = record_elem_in;
				}
				insert_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, key, record);
			}

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;
		}
		else if (strcmp(command, "R") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			reorganise(index_file_ptr, primary_file_ptr, overflow_file_ptr);

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;
		}
		else if (strcmp(command, "U") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			int old_key_in, new_key_in;
			fscanf_s(test_file, "%d %d", &old_key_in, &new_key_in);
			key_type old_key = old_key_in, new_key = new_key_in;

			record_elem_type new_record[RECORD_ELEM_NUM];

			for (int i = 0; i < RECORD_ELEM_NUM; i++)
			{
				int record_elem_in;
				fscanf_s(test_file, "%d", &record_elem_in);
				new_record[i] = record_elem_in;
			}

			if (old_key == new_key)
			{
				update_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, old_key, new_record);
			}
			else
			{
				Flag update_flag = delete_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, old_key);

				if (update_flag == WRONG_KEY)
				{
				}
				else if (update_flag == WRONG_KEY_ROOT_KEY)
				{
				}
				else if (update_flag == WRONG_KEY_NULL_KEY)
				{
				}
				else
				{
					insert_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, new_key, new_record);
				}
			}

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;
		}
		else if (strcmp(command, "D") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			int key_in;
			fscanf_s(test_file, "%d", &key_in);
			key_type key = key_in;

			delete_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, key);

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;
		}
	}
}

void console_input_mode(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr)
{
	printf("Below type in operations or the command `help` or `menu`\n");
	
	while (1)
	{
		char command[COMMAND_MAX_LEN];
		printf("C> ");
		scanf_s("%s", command, COMMAND_MAX_LEN);
		if (strcmp(command, "help") == 0)
		{
			printf("Help:\n\n");
			printf("I key r_0 r_1 ... r_%d - inserts a record with a given key and elements\n\n", RECORD_ELEM_NUM - 1);
			printf("R - reorganises\n\n");
			printf("P - prints all of the files\n\n");
			printf("D key - deletes record with given key (UNIMPLEMENTED)\n\n");
			printf("U old_key new_key r_0 r_1 ... r_%d - updates a record with a given key with new elements\n\n", RECORD_ELEM_NUM - 1);
			printf("menu - goes back to the menu\n\n");
		}
		else if (strcmp(command, "I") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			int key_in; 
			scanf_s("%d", &key_in);
			key_type key = key_in; 

			record_elem_type record[RECORD_ELEM_NUM];

			for (int i = 0; i < RECORD_ELEM_NUM; i++)
			{
				int record_elem_in;
				scanf_s("%d", &record_elem_in);
				record[i] = record_elem_in;
			}

			Flag insert_flag = insert_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, key, record);

			if (insert_flag == WRONG_KEY)
			{
				printf("Record with key %d already exists!\n", key_in);
			}
			else if (insert_flag == REORGANISED)
			{
				printf("Reorganising...\n");
			}

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;

			printf("Reads: %d, Writes: %d\n", local_reads, local_writes);
		}
		else if (strcmp(command, "R") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			reorganise(index_file_ptr, primary_file_ptr, overflow_file_ptr);

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;

			printf("Reads: %d, Writes: %d\n", local_reads, local_writes);
		}
		else if (strcmp(command, "P") == 0)
		{
			print_file(*index_file_ptr);
			print_file(*primary_file_ptr);
			print_file(*overflow_file_ptr);
		}
		else if (strcmp(command, "U") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			int old_key_in, new_key_in;
			scanf_s("%d %d", &old_key_in, &new_key_in);
			key_type old_key = old_key_in, new_key = new_key_in;

			record_elem_type new_record[RECORD_ELEM_NUM];

			for (int i = 0; i < RECORD_ELEM_NUM; i++)
			{
				int record_elem_in;
				scanf_s("%d", &record_elem_in);
				new_record[i] = record_elem_in;
			}

			if (old_key == new_key)
			{
				Flag update_flag = update_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, old_key, new_record);

				if (update_flag == WRONG_KEY)
				{
					printf("Record with key %d doesn't exist!\n", old_key);
				}
				else if (update_flag == WRONG_KEY_ROOT_KEY)
				{
					printf("Can't update the root record!\n");
				}
				else if (update_flag == WRONG_KEY_NULL_KEY)
				{
					printf("Key can't be NULL!\n");
				}
			}
			else
			{
				Flag update_flag = delete_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, old_key);

				if (update_flag == WRONG_KEY)
				{
					printf("Record with key %d doesn't exist!\n", old_key_in);
				}
				else if (update_flag == WRONG_KEY_ROOT_KEY)
				{
					printf("Can't update the root record!\n");
				}
				else if (update_flag == WRONG_KEY_NULL_KEY)
				{
					printf("Key can't be NULL!\n");
				}
				else
				{
					insert_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, new_key, new_record);
				}
			}

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;

			printf("Reads: %d, Writes: %d\n", local_reads, local_writes);
		}
		else if (strcmp(command, "D") == 0)
		{
			reset_buffer_stats((*index_file_ptr)->buffer);
			reset_buffer_stats((*primary_file_ptr)->buffer);
			reset_buffer_stats((*overflow_file_ptr)->buffer);

			int key_in;
			scanf_s("%d", &key_in);
			key_type key = key_in;

			Flag delete_flag = delete_record(index_file_ptr, primary_file_ptr, overflow_file_ptr, key);

			if (delete_flag == WRONG_KEY)
			{
				printf("Record with key %d doesn't exist!\n", key_in);
			}
			else if (delete_flag == WRONG_KEY_ROOT_KEY)
			{
				printf("Can't delete the root record!\n");
			}
			else if (delete_flag == WRONG_KEY_NULL_KEY)
			{
				printf("Key can't be NULL!\n");
			}

			int local_reads, local_writes;
			local_reads = (*index_file_ptr)->buffer->reads + (*primary_file_ptr)->buffer->reads + (*overflow_file_ptr)->buffer->reads;
			local_writes = (*index_file_ptr)->buffer->writes + (*primary_file_ptr)->buffer->writes + (*overflow_file_ptr)->buffer->writes;

			reads += local_reads;
			writes += local_writes;

			printf("Reads: %d, Writes: %d\n", local_reads, local_writes);
		}
		else if (strcmp(command, "menu") == 0)
		{
			break;
		}
	}

	printf("Global Reads: %d, Global Writes: %d\n\n", reads, writes);
}

void test_file_input_mode(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr)
{
	printf("Below type in operations or the command `help` or `menu`\n");
	char command[COMMAND_MAX_LEN];

	while (1)
	{
		printf("T> ");
		scanf_s("%s", command, COMMAND_MAX_LEN);
		if (strcmp(command, "help") == 0)
		{
			printf("Help:\n\n");
			printf("T file_name - runs a test file with given file name\n\n");
			printf("P - prints all of the files\n\n");
			printf("menu - goes back to the menu\n\n");
		}
		else if (strcmp(command, "T") == 0)
		{
			char file_name[MAX_FILE_PATH_LEN];
			scanf_s("%s", file_name, MAX_FILE_PATH_LEN);

			run_test_file(file_name, index_file_ptr, primary_file_ptr, overflow_file_ptr);
		}
		else if (strcmp(command, "P") == 0)
		{
			print_file(*index_file_ptr);
			print_file(*primary_file_ptr);
			print_file(*overflow_file_ptr);
		}
		else if (strcmp(command, "menu") == 0)
		{
			break;
		}
	}

	printf("Global Reads: %d, Global Writes: %d\n\n", reads, writes);
}

void menu(DiskFile** index_file_ptr, DiskFile** primary_file_ptr, DiskFile** overflow_file_ptr)
{
	while (1)
	{
		printf("Choose input mode:\n");
		printf("1. Console (1)\n");
		printf("2. Test File (2)\n");
		printf("3. End program (3)\n");
		printf("> ");

		int mode;

		scanf_s("%d", &mode);
		switch (mode)
		{
		case 1:
		{
			printf("Console has been chosen\n");
			console_input_mode(index_file_ptr, primary_file_ptr, overflow_file_ptr);
			break;
		}
		case 2:
		{
			printf("Test file has been chosen\n");
			test_file_input_mode(index_file_ptr, primary_file_ptr, overflow_file_ptr);
			break;
		}
		case 3:
		{
			printf("Exiting...\n");
			return;
		}
		}
	}
}

int main()
{
	//srand(0);
	//srand(time(0));
	system("color 07");
	
	DiskFile* index_file;
	DiskFile* primary_file;
	DiskFile* overflow_file;

	create_disk_file(&index_file, "index_file_0", INDEX);
	create_disk_file(&primary_file, "primary_file_0", DATA);
	create_disk_file(&overflow_file, "overflow_file_0", DATA);

	init_diskfiles(index_file, primary_file, overflow_file);

	/*record_elem_type record[RECORD_ELEM_NUM] = { 10,11,12,13 };
	insert_record(&index_file, &primary_file, &overflow_file, 5, record);
	insert_record(&index_file, &primary_file, &overflow_file, 10, record);
	insert_record(&index_file, &primary_file, &overflow_file, 100, record);
	insert_record(&index_file, &primary_file, &overflow_file, 20, record);
	insert_record(&index_file, &primary_file, &overflow_file, 30, record);
	insert_record(&index_file, &primary_file, &overflow_file, 18, record);
	insert_record(&index_file, &primary_file, &overflow_file, 50, record);
	insert_record(&index_file, &primary_file, &overflow_file, 60, record);
	insert_record(&index_file, &primary_file, &overflow_file, 70, record);
	insert_record(&index_file, &primary_file, &overflow_file, 80, record);
	insert_record(&index_file, &primary_file, &overflow_file, 55, record);
	insert_record(&index_file, &primary_file, &overflow_file, 9, record);
	insert_record(&index_file, &primary_file, &overflow_file, 8, record);
	insert_record(&index_file, &primary_file, &overflow_file, 6, record);
	insert_record(&index_file, &primary_file, &overflow_file, 7, record);
	insert_record(&index_file, &primary_file, &overflow_file, 120, record);
	insert_record(&index_file, &primary_file, &overflow_file, 2, record);
	insert_record(&index_file, &primary_file, &overflow_file, 75, record);
	insert_record(&index_file, &primary_file, &overflow_file, 29, record);
	insert_record(&index_file, &primary_file, &overflow_file, 130, record);
	insert_record(&index_file, &primary_file, &overflow_file, 110, record);
	insert_record(&index_file, &primary_file, &overflow_file, 99, record);
	insert_record(&index_file, &primary_file, &overflow_file, 98, record);
	insert_record(&index_file, &primary_file, &overflow_file, 97, record);
	insert_record(&index_file, &primary_file, &overflow_file, 11, record);
	insert_record(&index_file, &primary_file, &overflow_file, 12, record);
	insert_record(&index_file, &primary_file, &overflow_file, 3, record);*/

	menu(&index_file, &primary_file, &overflow_file);

	delete_disk_file(index_file);
	delete_disk_file(primary_file);
	delete_disk_file(overflow_file);
}