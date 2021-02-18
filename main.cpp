#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

int main()
{
    //Getting directory path
    std::vector<std::string> files_list;
    std::string files_path;

    std::cout << "Enter directory path: " << std::endl;
    std::cin >> files_path;

    //Getting names of all .csv files in specified directory
    for (const auto &entry : std::filesystem::directory_iterator(files_path))
        if (entry.path().extension() == ".csv")
            files_list.push_back(entry.path());

    //Counting points for commands
    std::string current_line;
    std::ifstream current_file;
    int commands_number, command_points;

    for (size_t i = 0; i < files_list.size(); i++)
    {
        current_file.open(files_list[i]);
        std::getline(current_file, current_line);
        commands_number = std::stoi(current_line);
        for (size_t i = 0; i < commands_number; i++)
        {
            std::getline(current_file, current_line);
            std::string command_name = current_line.substr(0, current_line.find(','));
            std::cout << command_name << std::endl;
        }
        current_file.close();
    }

    return 0;
}