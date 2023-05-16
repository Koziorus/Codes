#include <windows.h>
#include <stdio.h>
#include <time.h>

#define OK "-1"
#define ALREADY_USED "-2"

#define BUFFER_SIZE 100
#define MAX_NUMBER 1000
#define DELAY 20

#define PIPE_NAME "\\\\.\\pipe\\MyPipe"

int check_if_full(int* used_numbers)
{
	for(int i = 0; i < MAX_NUMBER; i++)
	{
		if(used_numbers[i] == 0)
		{
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[])
{
    int used_numbers[MAX_NUMBER];
    for (int i = 0; i < MAX_NUMBER; i++)
    {
        used_numbers[i] = 0;
    }

    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
	
	DWORD write_bytes_n;
	DWORD read_bytes_n;
	
    if (argc == 1)
    {
		srand(time(0) + 1000);

        CreateProcess(
            NULL,
            "main.exe -child",
            NULL,
            NULL,
            FALSE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi
        );
		
		WSADATA wsa;
		SOCKET server_fd, new_socket;
		struct sockaddr_in address;
		int opt = 1;
		int addrlen = sizeof(address);
		char buffer[1024] = {0};
		char message[1024];

		WSAStartup(MAKEWORD(2, 2), &wsa);

		server_fd = socket(AF_INET, SOCK_STREAM, 0);

		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));


		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

		bind(server_fd, (struct sockaddr *)&address, sizeof(address));

		listen(server_fd, 3);

        new_socket = accept(server_fd, (struct sockaddr *)&address, (int *)&addrlen);

        while (check_if_full(used_numbers) == 0)
        {
			char in_number[BUFFER_SIZE];
			
			recv(new_socket, in_number, 1024, 0);
			
			int received_number = atoi(in_number);
			
			char out_answer[10];
			
			if(used_numbers[received_number] == 0)
			{
				strcpy(out_answer, OK);
			}
			else
			{
				strcpy(out_answer, ALREADY_USED);
			}
			
			send(new_socket, out_answer, strlen(message), 0);
			
            char out_number[10];

			int random_number = rand() % MAX_NUMBER;
			
			while(used_numbers[random_number] == 1)
			{
				random_number = rand() % MAX_NUMBER;
			}
			
			sprintf(out_number, "%d", random_number);

            send(new_socket, out_number, strlen(message), 0);
			

			char in_answer[BUFFER_SIZE];

            recv(new_socket, in_answer, 1024, 0);


			if(!strcmp(in_answer, OK))
			{
				used_numbers[random_number] = 1;
				printf("%d\n", random_number);

			}
			else if(!strcmp(in_answer,ALREADY_USED))
			{
				used_numbers[random_number] = 1;
				printf("already used\n");
			}

            Sleep(DELAY);
        }
		
		CloseHandle(hPipe);
    }
    else
    {
		printf("test");
		/* srand(time(0) + 2000);
        HANDLE hPipe = CreateFile(
            PIPE_NAME,         				// Pipe name
            GENERIC_READ |                // Read and write access
            GENERIC_WRITE,					
            0,                            // No sharing
            NULL,                         // Default security attributes
            OPEN_EXISTING,                // Opens existing pipe
            0,                            // Default attributes
            NULL                          // No template file
        );

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            fprintf(stderr, "Failed to connect to the named pipe. Error code: %lu\n", GetLastError());
            return 1;
        }

        printf("Connected to the named pipe\n");

        while (check_if_full(used_numbers) == 0)
        {
            char out_number[10];

			int random_number = rand() % MAX_NUMBER;
			
			while(used_numbers[random_number] == 1)
			{
				random_number = rand() % MAX_NUMBER;
			}
			
			sprintf(out_number, "%d", random_number);

            if(!WriteFile(hPipe, out_number, sizeof(out_number), &write_bytes_n, NULL))
			{
				// pipe ended
				break;
			}

            char in_answer[BUFFER_SIZE];

            ReadFile(hPipe, in_answer, sizeof(in_answer) - 1, &read_bytes_n, NULL);

		
			if(!strcmp(in_answer, OK))
			{
				used_numbers[random_number] = 1;
				printf("%d\n", random_number);

			}
			else if(!strcmp(in_answer,ALREADY_USED))
			{
				used_numbers[random_number] = 1;
				printf("already used\n");
			}
			
			char in_number[BUFFER_SIZE];

			ReadFile(hPipe, in_number, sizeof(in_number) - 1, &read_bytes_n, NULL);
			
			int received_number = atoi(in_number);
			
			char out_answer[10];
			
			if(used_numbers[received_number] == 0)
			{
				strcpy(out_answer, OK);
			}
			else
			{
				strcpy(out_answer, ALREADY_USED);
			}
			
			WriteFile(hPipe, out_answer, sizeof(out_answer), &write_bytes_n, NULL);
        }
		
        CloseHandle(hPipe); */
    }
	
	printf("END\n");

    getchar();

    return 0;
}


