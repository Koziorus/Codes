#include <string>
#include <fstream>

#include "plot_helper.h"


void out_array(double* arr, int n, std::string file_path)
{
    std::ofstream data_file(file_path);
    for(int i = 0; i < n; ++i)
    {
        data_file << arr[i] << "\n";
    }
    data_file.close();
}

void plot(std::string parameters)
{
    std::string command = "matlab -nodesktop -nosplash -noFigureWindows -r ";
    std::string system_command = command + "\"" + parameters + "quit;" + "\"";

    system(system_command.c_str());
}