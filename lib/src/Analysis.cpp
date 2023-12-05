//
// Created by Krithika Natarajan on 12/4/23.
//
#include <iostream>
#include <string>
#include <fstream>

#include "data_processor.h"
#include "visualization.h"
#include "../matplot/matplot.h"
#include "Analysis.h"
#include <vector>
#include <variant>
#include <map>
#include <algorithm>
#include <optional>

Analysis::Analysis() {

}

Analysis::~Analysis(){

}
void Analysis::unit_standardization(data_processor& dataset) {
    /* This function standardizes units for GHG emissions
     * Parameter:
     * data_processor - data set to standardize
     * */

    for (auto& record : dataset.data_map) {
        auto it = record.find("GHG Emissions (mt CO2e)");
        if (it != record.end() && std::holds_alternative<double>(it->second)) {
            it->second = std::get<double>(it->second) / 1000;  // Convert t CO2e to mt CO2e
            }
        }
    }



data_processor Analysis::combine_dataset(data_processor set_1, data_processor set_2) {
    /* This function combines two datasets into one by looking at the common headers
     * Parameter:
     * data_processor - datasets one and two
     * */

    //convert the units to mtCO2E
    unit_standardization(set_1);

    //check which headers from each dataset are the same and extract those headers
    std::vector<std::string> set_1_headers = set_1.get_headers();
    std::vector<std::string> set_2_headers = set_2.get_headers();

    //change set_1 header name to match the headers
    for (size_t i = 0; i < set_2_headers.size(); ++i) {
        if (set_2_headers[i] == "GHG Emissions (t CO2e)") {
            set_2_headers[i] = "GHG Emissions (mt CO2e)";
            break; // Assuming each header name is unique and you found the header to change
        }
    }

    //find common headers
    std::sort(set_1_headers.begin(), set_1_headers.end());
    std::sort(set_2_headers.begin(), set_2_headers.end());
    std::vector<std::string> common_headers;
    //add headers to new data map
    // Find the intersection of the two sets
    std::set_intersection(
            set_1_headers.begin(), set_1_headers.end(),
            set_2_headers.begin(), set_2_headers.end(),
            std::back_inserter(common_headers));



    data_processor combined_sets = set_1.add_data(set_2,common_headers);
    return combined_sets;
}




void Analysis::Community_Greenhouse_gas(){
    /* THis function conducts overview Analysis on the Community Greenhouse Gas CSV
     * */
    data_processor comm_greenhouse_gas;
    visualization visualizer;

    //COMMUNITY GREENHOUSE GAS DATA TYPES
    std::map<std::string, std::string> COL_TYPES = {
            {"Year (Calendar Year)", "double"},
            {"GHG Emissions (mt CO2e)", "double"}
    };

    comm_greenhouse_gas.read_data("../data/community-greenhouse-gas.csv", COL_TYPES);

    //LOCAL GOVERNMENT GREENHOUSE GAS
    std::map<std::string, std::string> COL_TYPES_1 = {
            {"Year (Fiscal Year)", "double"},
            {"GHG Emissions (t CO2e)", "double"}
    };
    std::map<std::string, std::string> col_types, std::map<std::string, std::pair<std::string, std::variant<int, double, std::string>>>&{
            {"GHG Emissions (t CO2e)", {"-"}}
    };
    data_processor local_gov_greenhouse_gas;
    local_gov_greenhouse_gas.read_data("../data/local-government-operations-greenhouse-gas.csv", COL_TYPES_1);

    //local_gov_greenhouse_gas.print_data();

    //COMBINE DATASETS USING SIMILAR HEADERS



    //HISTOGRAM ANALYSIS
    //data_processor cilr_data = comm_greenhouse_gas.filter_data("Sector", "Commercial/Industrial/Large Residential");
    //std::vector<double> Commercial_Industrial_Large_Residential = cilr_data.extract_column<double>("GHG Emissions (mt CO2e)");



    //visualizer.histogram(Commercial_Industrial_Large_Residential);


}