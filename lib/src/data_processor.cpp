//
// Created by Krithika Natarajan on 11/29/23.

#include "data_processor.h"
#include "csv.h"

#include <iostream>
#include <sstream>
#include <map>
#include <variant>
#include "matplot/matplot.h"
#include <vector>


data_processor::data_processor() {

}
data_processor::data_processor(std::vector<std::map<std::string, std::variant<int, double, std::string>>> data){
    data_map = data;
}

data_processor::~data_processor() {

}


void data_processor::read_data(std::string csv_path, std::map<std::string, std::string> col_types) {
    io::LineReader in(csv_path);

    // Storing the column names
    std::string header = in.next_line();
    std::stringstream headerStream(header);
    std::string headerCell;

    while (std::getline(headerStream, headerCell, ',')) {
        headers.push_back(headerCell);
    }

    // Iterate through the CSV file
    while (char *line = in.next_line()) {
        std::map<std::string, std::variant<int, double, std::string>> data_row;
        size_t column_index = 0;
        std::stringstream line_stream(line);
        std::string line_cell;

        while (std::getline(line_stream, line_cell, ',')) {
            if (column_index < headers.size()) {
                const std::string& current_header = headers[column_index];
                if (col_types.find(current_header) != col_types.end()) {
                    if (col_types.at(current_header) == "int") {
                        data_row[current_header] = std::stoi(line_cell);
                    }
                    else if (col_types.at(current_header) == "double") {
                        data_row[current_header] = std::stod(line_cell);
                    }
                    else {
                        data_row[current_header] = line_cell;
                    }
                }
                else {
                    data_row[current_header] = line_cell; // Default to string
                }
                column_index++;
            }
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

std::vector<std::map<std::string, std::variant<int, double, std::string>>> data_processor::filter_data(std::string col_name,std::variant<int, double, std::string> col_value) {
    /* Given a column name and the associated value to filter by, this function returns
     * a new data map object filtering for the rows that have the desired value
     * */
    data_processor new_data;
    for (const auto &row: data_map) {
        auto col_iter = row.find(col_name);
        if (col_iter != row.end()) {
            // Check if this row has the specified value
            if (col_iter->second.index() == col_value.index() && col_iter->second == col_value){
                new_data.data_map.push_back(row);
            }
        }
    }
    return new_data.data_map;
}































