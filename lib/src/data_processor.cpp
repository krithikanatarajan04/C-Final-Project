//
// Created by Krithika Natarajan on 11/29/23.

#include "data_processor.h"
#include "csv.h"

#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <variant>
#include <vector>
#include <optional>



data_processor::data_processor() = default;
data_processor::data_processor(std::vector<std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>>>& data){
    data_map = std::move(data);
}

data_processor::~data_processor() = default;

std::variant<int,double,std::string,std::optional<int>,std::optional<double>,std::optional<std::string>> data_processor::typecast_value(
        std::string header,
        const std::string value){
    /* This function converts a string value to the desired type for a given column, taking into consideration if the value
     * should be changed or not
     * Parameters:
     * std::string header - current header to pull information from
     * std::string value - value in string form
     *
     * Returns:
     * std::variant - value with the correct type for a given data point
     * */

    //check if replacement is needed for value
    auto repl_it = REPLACEMENTS.find(header);
    if (repl_it != REPLACEMENTS.end() && value == repl_it->second.first){
        return repl_it->second.second;
    }
    else {
        //check if a column type was specified and apply that conversion
        auto type_it = COL_TYPES.find(header);
        if (type_it != COL_TYPES.end()) {
            const std::string& type = type_it->second;
            if (type == "int") {
                return std::stoi(value);
            } else if (type == "double") {
                return std::stod(value);
            } else {
                return value; // Default to string
            }
        }
    }
}


void data_processor::assign_data(std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>>& data_row,
                                 std::string line_cell, size_t column_index){
    /* This function adds each new data point to the corresponding map
    * Parameters:
    * std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>,
    * std::optional<std::string>>>& data row - reference to a std::map to add new key value pair to
    * std::string - data point in string form
    * size_t - column index to understand current header
    */

    //find what index column is on and store that header
    if (column_index < headers.size()){
        const std::string &current_header = headers[column_index];
        //add data point to map for the row
        data_row[current_header] = typecast_value(current_header, line_cell);
        }
    }



void data_processor::read_data(std::string csv_path, std::map<std::string, std::string> col_types,
                               const std::map<std::string, std::pair<std::string,std::variant<int, double, std::string, std::optional<int>,std::optional<double>,std::optional<std::string>>>> replacements) {
    /* This function takes a csv file and reads it into a data_processor object so it can be used for analysis
     * Parameters:
     * std::string - csv path
     * std::map<std::string, std::string> - the column types to assign for each column (column name, column type).
     * std::map<std::string, std::pair<std::string,std::variant<int, double, std::string, std::optional<int>,std::optional<double>,std::optional<std::string>>>> - if any of the values need to be replaced
     * (KEY - Column Name, VALUE - Pair:(value to REPLACE, value to replace WITH (this is of type std::variant))
     * */

    //read csv line by line
    io::LineReader in(csv_path);
    COL_TYPES = col_types;
    REPLACEMENTS = replacements;
    // Storing the column names
    std::string header = in.next_line();
    std::stringstream headerStream(header);
    std::string headerCell;

    while (std::getline(headerStream, headerCell, ',')) {
        headers.push_back(headerCell);
    }
    // Iterate through the CSV file
    while (char *line = in.next_line()) {
        // Initialize row of data and column counter
        std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>> data_row;
        size_t column_index = 0;
        std::stringstream line_stream(line);
        std::string line_cell;

        //traverse through data in each row
        while (std::getline(line_stream, line_cell, ',')) {
            assign_data(data_row,line_cell,column_index);
        }
        data_map.push_back(data_row);
    }
}





void data_processor::print_data() {
    /*This function prints the csv data
     * */
    for (const auto &data_row: data_map) {
        for (const auto &pair: data_row) {
            const std::string &key = pair.first;

            // Use std::visit to access the value based on its type in the variant
            std::cout << "Key: " << key << ", Value: ";

            std::visit([&](auto &&value) {
                using T = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<T, int>) {
                    std::cout << value << " (int)";
                } else if constexpr (std::is_same_v<T, double>) {
                    std::cout << value << " (double)";
                } else if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << value << " (string)";
                }
            }, pair.second);

            std::cout << std::endl;
        }

    }
}
































