//
// Created by Krithika Natarajan on 11/29/23.
//

#ifndef C_FINAL_PROJECT_DATA_PROCESSOR_H
#define C_FINAL_PROJECT_DATA_PROCESSOR_H
#include <string>
#include <map>
#include <variant>
#include <vector>

class data_processor {
private:
    std::vector<std::string> headers;
    std::vector<std::map<std::string, std::variant<int, double, std::string>>> data_map;


public:

    void read_data(std::string csv_path, std::map<std::string, std::string> col_types);
    void print_data();
    std::vector<double> extract_column(std::string col_name);
    data_processor();
    ~data_processor();

    void clean_data();
};


#endif //C_FINAL_PROJECT_DATA_PROCESSOR_H
