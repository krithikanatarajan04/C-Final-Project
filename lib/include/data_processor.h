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

class data_processor {
private:
    std::vector<std::string> headers;
    std::vector<std::map<std::string, std::variant<int, double, std::string>>> data_map;


public:

    void read_data(std::string csv_path, std::map<std::string, std::string> col_types);
    void print_data();

    data_processor(std::vector<std::map<std::string, std::variant<int, double, std::string>>>);
    template <typename T>
    std::vector<T> extract_column(std::string col_name){
        /* This function returns a column of data
         * Parameters:
         * std::string - column name
         * Returns:
         * std::vector - column of data
         * */
        std::vector<T> col_data;
        for (const auto& row : data_map) {
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

    data_processor filter_data(std::string col_name,std::variant<int, double, std::string> col_value);

    data_processor();
    ~data_processor();

    void clean_data();
};


#endif //C_FINAL_PROJECT_DATA_PROCESSOR_H
