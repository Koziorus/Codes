#include "pr_thread.hpp"
#include <pthread.h>
#include <iostream>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

void* add_to_map(void* ptr)
{
    map_args* args = (map_args*)(ptr);
    std::string key = args -> key, value = args -> value;

    pthread_mutex_lock( &myMutex );
    std::cout<<"Trying to add "<<key<<" "<<"value -> ";

    if(args->ext_map->map_x.count(key) == 0)
    {
        args->ext_map->map_x.insert(std::pair<std::string, std::string>(key, value));
        std::cout<<"ADDED";
    }
    else
    {
        std::cout<<"ALREADY EXISTS";
    }

    std::cout<<"\n";
    pthread_mutex_unlock( &myMutex );
	
	return NULL;
}