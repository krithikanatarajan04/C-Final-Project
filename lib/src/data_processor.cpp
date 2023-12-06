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
template <typename T>
bool data_processor::check_variant_type(const std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>> variant) {
    return std::holds_alternative<T>(variant);
}



template <typename T>
data_processor data_processor::filter_data(const std::string& col_name, const T& col_value, bool exclude) {
    /* This function filters data for a certain value in a given column.
     * It can either save those rows or remove those rows into a new data object
     * Parameters:
     * std::string col_name - name of the column to search through
     * const& T - the value to filter by (can be any of supported variants)
     * bool exclude (automatically set to false) - to execlude rows with given value or include rows with given value
     *
     * Returns:
     * data_processor - new data_map that filters desired values
     * */
    data_processor new_data;
    bool is_value;
    //iterate through current map
    for (const auto& row : data_map) {
        auto col_iter = row.find(col_name);
        if (col_iter != row.end()) {
            //check the type of each variant and sees if it is equal
            //if types are equal, check if the values itself are equal
            std::visit([&](const auto& variant_value) {
                using VariantType = std::decay_t<decltype(variant_value)>; // Deduced type in the variant
                using ValueType = std::decay_t<decltype(col_value)>;      // Type of col_value

                // Compare only if the types are the same and includes or excludes accordingly
                if constexpr (std::is_same_v<VariantType, ValueType>) {
                    bool valueMatches = (variant_value == col_value);

                    if ((valueMatches && !exclude) || (!valueMatches && exclude)) {
                        new_data.data_map.push_back(row);
                    }
                }
            }, col_iter->second);
        }
    }
    return new_data;
}

// Explicit instantiations
template data_processor data_processor::filter_data<std::string>(const std::string&, const std::string&, bool);


template data_processor data_processor::filter_data<int>(const std::string&, const int&, bool);


template data_processor data_processor::filter_data<double>(const std::string&, const double&, bool);


template data_processor data_processor::filter_data<std::optional<int>>(const std::string&, const std::optional<int>&, bool);


template data_processor data_processor::filter_data<std::optional<double>>(const std::string&, const std::optional<double>&, bool);


template data_processor data_processor::filter_data<std::optional<std::string>>(const std::string&, const std::optional<std::string>&, bool);

std::vector<std::string> data_processor::get_headers(){
    /* This function returns the headers for a data set
     * */
    return headers;
}

std::map<std::string, std::string> data_processor::get_col_types(){
    return COL_TYPES;
}

std::map<std::string, std::pair<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>>> data_processor::get_replacement_map(){
    return REPLACEMENTS;
}



































