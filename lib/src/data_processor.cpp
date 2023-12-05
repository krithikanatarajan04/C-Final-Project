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
            std::cout << type <<std::endl;
            if (type == "int") {
                return std::stoi(value);
            } else if (type == "double") {
                return std::stod(value);
            } else {
                return value; // Default to string
            }
        }
        else{
            return value; //no type specified so automatically returns as a string
        }
    }
}

std::vector<std::string> data_processor::parse_csv_line(const std::string& line){
    std::vector<std::string> fields;
    std::string field;
    bool in_quotes = false;

    for (char ch : line) {
        if (ch == '"') {
            in_quotes = !in_quotes;  // Toggle the in_quotes flag
        } else if (ch == ',' && !in_quotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field.push_back(ch);
        }
    }
    fields.push_back(field);  // Add the last field
    return fields;
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
    //initialize CSV Reader object
    //fixed maximum columns
    const size_t MAX_COLUMNS = 10;

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

        std::vector<std::string> row = parse_csv_line(line);
            for (size_t i = 0; i < headers.size(); ++i) {
                //std::cout << "Header: " <<headers[i] << std::endl;
                //std::cout <<  "Line Cell: " << row[i] << std::endl;
                data_row[headers[i]] = typecast_value(headers[i], row[i]);
            }
        data_map.push_back(data_row);
    }
}





void data_processor::print_data() {
    /*This function prints the csv data in a readable format
     * */
    int idx=0;
    for (const auto &data_row: data_map) {
        std::cout << "Row: " << idx << " | ";
        for (const auto &pair: data_row) {
            const std::string &key = pair.first;
            const auto & val = pair.second;
            std::cout << key << ": ";
            if (const auto *value = std::get_if<std::string>(&val)) {
                std::cout << *value;
            } else if (const auto *value = std::get_if<int>(&val)) {
                std::cout << *value;
            } else if (const auto *value = std::get_if<double>(&val)) {
                std::cout << *value;
            } else if (const auto *value = std::get_if<std::optional<int>>(&val)) {
                if (value->has_value()) {
                    std::cout << value->value();
                } else {
                    std::cout << "N/A";
                }
            } else if (const auto *value = std::get_if<std::optional<double>>(&val)) {
                if (value->has_value()) {
                    std::cout << value->value();
                } else {
                    std::cout << "N/A";
                }
            } else if (const auto *value = std::get_if<std::optional<std::string>>(&val)) {
                if (value->has_value()) {
                    std::cout << value->value();
                } else {
                    std::cout << "N/A";
                }
            }
            else {
                std::cout << "Unknown type";
            }

            std::cout << " | ";
        }
        std::cout << "" << std::endl;
        idx++;
        }
        std::cout << "" << std::endl;
    }

































