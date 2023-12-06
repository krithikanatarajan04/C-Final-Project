//
// Created by Krithika Natarajan on 11/29/23.
//

#ifndef C_FINAL_PROJECT_DATA_PROCESSOR_H
#define C_FINAL_PROJECT_DATA_PROCESSOR_H
#include <string>
#include <map>
#include <variant>
#include <vector>
#include <iostream>
#include <optional>

class data_processor {
private:
    std::vector<std::string> headers;
    std::map<std::string, std::string> COL_TYPES;
    std::map<std::string, std::pair<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>>> REPLACEMENTS;

    void assign_data(
            std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>> &data_row,
            std::string line_cell, size_t column_index);

    std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>
    typecast_value(std::string header, const std::string value);

public:
    std::map<std::string, std::string> get_col_types();

    std::vector<std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>>> data_map;

    std::vector<std::string> parse_csv_line(const std::string& line);
    void read_data(std::string csv_path,std::map<std::string, std::string> col_types, const std::map<std::string, std::pair<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string> >>> replacements = {});
    //map: key:column name, value: pair - (value to replace, new value(int,double,string, optional int double or string)



    void print_data();

    std::vector<std::string> get_headers();

    data_processor(std::vector<std::map<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>>>& data);

    template<typename T>
    std::vector<T> extract_column(std::string col_name) {
        /* This function returns a column of data
         * Parameters:
         * std::string - column name
         * Returns:
         * std::vector - column of data
         * */
        std::vector<T> col_data;
        for (const auto &row: data_map) {
            auto col_iter = row.find(col_name);
            if (col_iter == row.end()) {
                std::cerr << col_name << " not found in row\n";
                continue; // Skip this iteration
            }

            if (std::holds_alternative<T>(col_iter->second)) {
                col_data.push_back(std::get<T>(col_iter->second));
            }
        }
        return col_data;
    }

    template <typename T>
    data_processor filter_data(const std::string& col_name, const T& col_value, bool exclude=false);
    data_processor();
    ~data_processor();

    data_processor merge_data(data_processor new_data, std::vector<std::string> common_headers);

    void replace_data(size_t row_index, const std::string &col_name, const std::string &rep_val,
                      const std::variant<int, double, std::string> &new_val);

    template <typename T>
    bool check_variant_type(const std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>> variant);

    std::map<std::string, std::pair<std::string, std::variant<int, double, std::string, std::optional<int>, std::optional<double>, std::optional<std::string>>>> get_replacement_map();
};

#endif //C_FINAL_PROJECT_DATA_PROCESSOR_H
