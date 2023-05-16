#include <map>
#include <string>
#include <windows.h>

struct Ext_map
{
    std::map<std::string, std::string> map_x;
};

struct map_args
{
    Ext_map* ext_map;
    std::string key, value;
};

void* add_to_map(void* ptr);