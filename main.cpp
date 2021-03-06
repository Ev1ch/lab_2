#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

namespace fs = std::filesystem;

void get_files_names(std::vector<std::string> &, std::string);
void check_all_matches(int, int, int &, std::string, int&, int& );
std::string get_command_name(std::string);
void find_winner(std::string, int &, std::string &, int &, int& );
void get_personal_results(std::string, int, std::ofstream &);
void looking_for_winner(int, std::ifstream &, std::string, std::string &, std::ofstream &, int &, int& );
void write_winner(std::string, std::ofstream &);
bool dir_exists(const std::string &);

int main()
{
    // Getting directory path
    std::vector<std::string> files_list;
    std::string files_path = "results";
    std::cout << "Enter directory path: " << std::endl;
    std::cin >> files_path;

    std::ifstream current_file;
    std::ofstream result_file("result.txt");

    // Check if the directory exists
    if (!dir_exists(files_path))
    {
        std::cout << "There is no such a directory..." << std::endl;
        return 0;
    }

    // Getting names of all .csv files in specified directory
    get_files_names(files_list, files_path);

    // Checking all files
    std::string current_line,
        winner_name;
    int commands_number,
        winner_points = 0,
        winner_goals = 0 ;

    for (size_t i = 0; i < files_list.size(); i++)
    {
        current_file.open(files_list[i]);

        if (!current_file.is_open())
            std::cout << "Something went wrong with " << files_list[i] << "..." << std::endl;

        std::getline(current_file, current_line);

        // Getting commands number in this file
        int commands_number = std::stoi(current_line);

        // Checking all commands and find winner
        looking_for_winner(commands_number, current_file,
                           current_line, winner_name,
                           result_file, winner_points,
                           winner_goals);
        current_file.close();
    }
    write_winner(winner_name, result_file);

    return 0;
}

bool dir_exists(const std::string &files_path)
{
    return std::filesystem::is_directory(files_path);
}

void looking_for_winner(int commands_left,
                        std::ifstream &current_file,
                        std::string current_line,
                        std::string &winner_name,
                        std::ofstream &result_file,
                        int &winner_points,
                        int& winner_goals)
{
    if (commands_left <= 0)
        return;

    std::getline(current_file, current_line);
    int command_points = 0;
    std::string command_name = get_command_name(current_line);
    find_winner(current_line, winner_points, winner_name, command_points, winner_goals);

    get_personal_results(command_name, command_points, result_file);

    return looking_for_winner(commands_left - 1, current_file,
                              current_line, winner_name,
                              result_file, winner_points,
                              winner_goals);
}

std::string get_command_name(std::string current_line)
{
    return current_line.substr(0, current_line.find(','));
}

void get_files_names(std::vector<std::string> &files_list, std::string files_path)
{
    for (const auto &entry : std::filesystem::directory_iterator(files_path))
    {
        if (entry.path().extension() == ".csv")
        {
            fs::path path{entry};
            std::string path_string{path.string()};
            files_list.push_back(path_string);
        }
    }
}

void check_all_matches(int j, int len, int &command_points, std::string current_line, int& winner_goals, int& diff_goals)
{
    if (j >= len)
        return;

    int match_start = ++j,
        match_end = current_line.find(',', j);
    if (match_end == std::string::npos)
        match_end = current_line.length();
    std::string current_match = current_line.substr(match_start, match_end - match_start);

    // Getting current match goals table
    int command_goals = std::stoi(current_match.substr(0, current_match.find(':'))),
        opponent_goals = std::stoi(current_match.substr(current_match.find(':') + 1));

    //command_points += ((command_goals > opponent_goals) ? 3 : (command_goals == opponent_goals) ? 1: 0);
    if( command_goals > opponent_goals ){
        command_points += 3 ;
    } else if( command_goals == opponent_goals){
        command_points += 1 ;
    }
    diff_goals += (command_goals - opponent_goals) ;
    return check_all_matches(match_end, len, command_points, current_line, winner_goals, diff_goals);
}

void find_winner(std::string current_line, int &winner_points, std::string &winner_name, int &command_points, int& winner_goals)
{
    int diff_goals = 0 ;
    check_all_matches(current_line.find(','), current_line.length(), command_points, current_line, winner_goals, diff_goals);

    // Updating winner
    if ( command_points > winner_points )
        winner_points = command_points, winner_name = get_command_name(current_line), winner_goals = diff_goals;
    else if( command_points == winner_points ){
        if( diff_goals > winner_goals ){
            winner_points = command_points, winner_name = get_command_name(current_line), winner_goals = diff_goals;
        }
    }

}

void get_personal_results(std::string command_name, int command_points, std::ofstream &result_file)
{
    result_file << std::setw(30) << command_name + ":\t" << std::to_string(command_points) << std::endl;
}

void write_winner(std::string winner_name, std::ofstream &result_file)
{
    result_file << "----------------------------------------------------------------\n";
    result_file << std::setw(30) << "Winner: \t" << winner_name << '\n';
    result_file.close();
}
