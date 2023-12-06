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
#include <algorithm>



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


// Copy constructor
data_processor::data_processor(const data_processor& other) {
    this->data_map = other.data_map;
    this->COL_TYPES = other.COL_TYPES;
    this->headers = other.headers;
}

data_processor& data_processor::operator=(const data_processor& other){
    if (this != &other) { // Protect against self-assignment
        // Copy each member variable from 'other' to 'this'
        data_map = other.data_map;
        COL_TYPES = other.COL_TYPES;
        headers = other.headers;

        // For other resources (like dynamic memory), release the current resources
        // and allocate new ones, copying the content from 'other'
    }
    return *this;
}


// Define a helper function to compare values of different types
template <typename T, typename U>
bool compare_values(const T& col_value, const U& variant_value, bool exclude) {
    // Special handling for std::optional<T> and U comparison
    bool comparisonResult = false;

    // Use std::visit to handle different types inside the variant
    std::visit([&](const auto& value) {
        using VariantType = std::decay_t<decltype(value)>;

        // Check if the variant currently holds a type that matches T
        if constexpr (std::is_same_v<VariantType, T>) {
            comparisonResult = (value == col_value);
        }
    }, variant_value);

    return (comparisonResult != exclude);
}




template <typename T>
data_processor data_processor::filter_data(const std::string& col_name, const T& col_value, bool exclude) {
    /* This function filters data based on a column name a value in that column. It can either include or exclude that value
     * Parameters:
     * std::string - col name
     * col value - variable type
     * excluded(set to false) - whether or not to look for and add to or remove from data map
     * Returns:
     * data_processor
     * */
    data_processor new_data;

    for (const auto& row : data_map) {
        auto col_iter = row.find(col_name);

        if (col_iter != row.end()) {
            if (compare_values(col_value, col_iter->second, exclude)) {
                new_data.data_map.push_back(row);
            }
        }
    }
    new_data.COL_TYPES = this->COL_TYPES;
    new_data.headers = this->headers;
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

std::vector<std::string> find_common_headers(std::vector<std::string> head_1, std::vector<std::string> head_2){
    /* This function finds the common headers between two datasets
     * Parameters:
     * std::vector<std::string> - headers 1
     * std::vector<std::string> - headers 2
     * Returns::
     * std::vector<std::string> - commmon headers
     * */
    //check which headers from each dataset are the same and extract those headers


    //find common headers
    std::sort(head_1.begin(), head_1.end());
    std::sort(head_2.begin(), head_2.end());
    std::vector<std::string> common_headers;
    //add headers to new data map
    // Find the intersection of the two sets
    std::set_intersection(
            head_1.begin(), head_1.end(),
            head_2.begin(), head_2.end(),
            std::back_inserter(common_headers));

    return common_headers;
}

void data_processor::add_data(data_processor set, std::vector<std::string> common_headers){
    /*This function traverse through a dataset and adds it to another one
     * Parameters:
     * data processor - data
     * std::vector<std::string> common_headers - between the two datasets
     * */
    // Traverse through new_data's data_map
    for (const auto& row : set.data_map) {
        std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>> place_holder;
        // Process each header in the current row
        for (const auto& current_header : common_headers) {
            auto it = row.find(current_header);
            if (it != row.end()) {
                // Add the found data to the place_holder map
                place_holder[current_header] = it->second;
            }
        }
        // Add the processed row to the combined_sets data map
        this->data_map.push_back(place_holder);
    }
}

void data_processor::update_header(int header_idx, std::string new_header){
    /* This function takes in the header index and replaces the header at that spot with a new header
     * Parameters:
     * int header_idx - index to replace at
     * std::string new_header - what to replace header with
     * */

    //replace COL_TYPES (if needed)
    for(auto header : COL_TYPES){
        if(header.first == headers[header_idx]){
            COL_TYPES[new_header] = header.second;
        }
    }
    this->headers[header_idx] = new_header;
}

data_processor data_processor::merge_data(data_processor set_1, data_processor set_2) {
    /* This function adds a set of data to another data_set given a common set of headers
     * Parameters:
     * data_processer new_data - the new data to be added
     * Returns:
     * data_processor combined_sets - the new combined data_sets
     * */
    data_processor combined_sets;
    std::vector<std::string> set_1_headers = set_1.get_headers();
    std::vector<std::string> set_2_headers = set_2.get_headers();
    std::vector<std::string> common_headers = find_common_headers(set_1_headers, set_2_headers);


    combined_sets.add_data(set_1,common_headers);
    combined_sets.add_data(set_2,common_headers);

    // Initialize or validate COL_TYPES for combined_sets
    for (const auto& header : common_headers) {
        auto it1 = set_1.COL_TYPES.find(header);
        auto it2 = set_2.COL_TYPES.find(header);

        // Check if the header exists in both set_1 and set_2 COL_TYPES
        if (it1 != set_1.COL_TYPES.end() && it2 != set_2.COL_TYPES.end()){
            combined_sets.COL_TYPES[header] = it1->second;
        }
    }
    combined_sets.headers = common_headers;
    return combined_sets;
}

template <typename T>
void data_processor::add_col(const std::vector<T>& vec, const std::string& col_name){
    /* This function adds a column to datamap
     * Parameters:
     * std:Vector<T> vec - data to add
     * std::string col name - name of column (or key)
     * */

    // Ensure data_map has enough rows to add new column data

    if (this->data_map.size() < vec.size()) {
        // Expand the data_map to accommodate new rows
        this->data_map.resize(vec.size());
    }

    // Iterate through the vector and add more column data in each row
    for (size_t i = 0; i < vec.size(); i++) {
        this->data_map[i][col_name] = vec[i];
    }
}

// Explicit instantiations
template void data_processor::add_col<int>(const std::vector<int>&, const std::string&);
template void data_processor::add_col<double>(const std::vector<double>&, const std::string&);
template void data_processor::add_col<std::string>(const std::vector<std::string>&, const std::string&);


data_processor data_processor::aggregation(const std::vector<std::string>& col_names, const std::string& sum_col) {
    /* This function aggregates data based on columns provided
     * Parameters:
     * std::vector<std::string>> col names
     * std::string - column to sum on
     * Returns:
     * data_processor
     * */
    std::vector<std::string> all_cols;
    all_cols = col_names;
    all_cols.push_back(sum_col);

    data_processor sub_data;
    for (const auto& col_name : all_cols) {
        auto it = COL_TYPES.find(col_name);
        if (it != COL_TYPES.end()) {
            if (it->second == "int") {
                std::vector<int> int_vec = extract_column<int>(col_name);
                sub_data.add_col(int_vec, col_name);
            }
            else if (it->second == "double") {
                std::vector<double> double_vec = extract_column<double>(col_name);
                sub_data.add_col(double_vec, col_name);
            } else {
                std::vector<std::string> string_vec = extract_column<std::string>(col_name);
                sub_data.add_col(string_vec, col_name);
            }
        } else {
            std::vector<std::string> default_vec = extract_column<std::string>(col_name);
            sub_data.add_col(default_vec, col_name);
        }
    }

    // sum by specified column
    data_processor aggregate;

    for (auto row : sub_data.data_map) {
        bool is_there = false;

        // Iterate through aggregate data_map to check if the value combo is already there
        for (auto& r : aggregate.data_map) {
            bool match = true;

            // Compare all keys EXCEPT THE SUM COLUMN
            for (auto col : col_names) {
                auto big = row.find(col);
                auto a = r.find(col);

                if (a != r.end() && big != row.end() && a->second != big->second) {
                    match = false;
                    break;
                }
            }

            if (match) {
                // Perform addition based on the actual types within the variants
                if (r[sum_col].index() == 0 && row[sum_col].index() == 0) {

                    std::get<int>(r[sum_col]) += std::get<int>(row[sum_col]);
                } else if (r[sum_col].index() == 1 && row[sum_col].index() == 1) {

                    std::get<double>(r[sum_col]) += std::get<double>(row[sum_col]);
                }

                is_there = true;
                break;
            }
        }

        if (!is_there) {
            // Insert a new data row into aggregate
            std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>> place;
            for (auto col : all_cols) {
                place[col] = row[col];
            }
            aggregate.data_map.push_back(place);
        }
    }


    return aggregate;

}
data_processor data_processor::get_subset(const std::vector<std::string>& col_names){
    /* This function takes in the desired columns and returns a subset of a larger data map
     * Parameters:
     * std::string - column names
     * Returns:
     * data processor
     * */
    data_processor subset;
    //traverse through columns
    for(auto col : col_names){
        auto it = COL_TYPES.find(col);
        if( it != COL_TYPES.end()){
            if(it->second == "int"){
                subset.add_col(this->extract_column<int>(col), col);
            }
            else if(it->second == "double"){
                subset.add_col(this->extract_column<double>(col), col);
            }
            else{
                subset.add_col(this->extract_column<std::string>(col), col);
            }
        }
        else{
            subset.add_col(this->extract_column<std::string>(col), col);
        }
    }
    subset.headers = col_names;
    subset.COL_TYPES = this->COL_TYPES;
    return subset;
}




































