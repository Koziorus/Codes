#include "pr_thread.hpp"
#include <iostream>
#include <pthread.h>

int main()
{
    int n;
    std::cin>>n;
    pthread_t* threads_IDs = new pthread_t[n];
    map_args** arguments = new map_args*[n];
    char key_i = '!', value_i = 'a';

    Ext_map* ext_map = new Ext_map;

    for(int i = 0; i < n; i++)
    {
        int random = rand() % 100;

        arguments[i] = new map_args;
        arguments[i]->key = (key_i + random);
        arguments[i]->value = value_i + i;
        arguments[i]->ext_map = ext_map;

        //, reinterpret_cast<const pthread_attr_t *>(arguments[i])
        pthread_create(&threads_IDs[i], NULL, add_to_map, arguments[i]);
    }

    for(int i = 0; i < n; i++)
    {
        pthread_join(threads_IDs[i], NULL);
    }
    
    return 0;
}
