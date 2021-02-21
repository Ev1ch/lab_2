// ! TODO: decompose to functions, look for ability to make optimisation

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void getNames( std::vector<std::string>& , std::string ) ;
void CheckingAllMatches(int, int, int&, std::string );

int main()
{
    // Getting directory path
    std::vector<std::string> files_list;
    std::string files_path = "results";
    std::cout << "Enter directory path: " << std::endl;
    std::cin >> files_path;

    // Getting names of all .csv files in specified directory
    getNames(files_list, files_path) ;

    // Checking all files
    std::string current_line,
        winner_name;
    int commands_number,
        winner_points = 0;
    std::ifstream current_file;
    std::ofstream result_file("result.txt");

    for (size_t i = 0; i < files_list.size(); i++)
    {
        current_file.open(files_list[i]);
        std::getline(current_file, current_line);

        // Getting commands number in this file
        commands_number = std::stoi(current_line);

        // Checking all commands and find winner
        for (size_t i = 0; i < commands_number; i++)
        {
            std::getline(current_file, current_line);
            size_t j = current_line.find(',');
            int command_points = 0;
            std::string command_name = current_line.substr(0, j),
                        result_line;

            CheckingAllMatches(j, current_line.length(), command_points, current_line) ;

            // Updating winner
            if (command_points >= winner_points){
                winner_points = command_points, winner_name = command_name;
            }

            // Writing command points in file
            result_file << std::setw(30) << command_name + ":"
                        << "\t" << std::to_string(command_points) << std::endl;
        }
        current_file.close();
    }

    // Writing winner
    result_file << "----------------------------------------------------------------" << std::endl;
    result_file << std::setw(30) << "Winner: " << '\t' << winner_name << '\n' ;
    result_file.close();

    return 0;
}
void getNames( std::vector<std::string>& files_list, std::string files_path){
    for (const auto &entry : std::filesystem::directory_iterator(files_path)){
        if (entry.path().extension() == ".csv"){
            fs::path path{entry};
            std::string path_string{path.string()};
            files_list.push_back(path_string) ;
        }
    }
}

void CheckingAllMatches(int j, int len, int& command_points, std::string current_line ){
    if( j >= len )return ;

    int match_start = ++j,
        match_end = current_line.find(',', j);
    if (match_end == std::string::npos)
        match_end = current_line.length();
    std::string current_match = current_line.substr(match_start, match_end - match_start);

    // Getting current match goals table
    int command_goals = std::stoi(current_match.substr(0, current_match.find(':'))),
        opponent_goals = std::stoi(current_match.substr(current_match.find(':') + 1));

    command_points += ((command_goals > opponent_goals) ? 3 : (command_goals == opponent_goals) ? 1 : 0 )  ;

    return CheckingAllMatches(match_end, len, command_points, current_line) ;
}
