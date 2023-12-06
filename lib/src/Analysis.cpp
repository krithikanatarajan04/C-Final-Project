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
        auto it = record.find("GHG Emissions (t CO2e)");
        if (it != record.end() && std::holds_alternative<double>(it->second)) {
            it->second = std::get<double>(it->second) / 1000;  // Convert mt CO2e to mt CO2e
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
    for (size_t i = 0; i < set_1_headers.size(); ++i) {
        if (set_1_headers[i] == "GHG Emissions (t CO2e)") {
            set_1.update_header(i,"GHG Emissions (mt CO2e)");

            break; // Assuming each header name is unique and you found the header to change
        }
    }

    data_processor combine_sets = data_processor::merge_data(set_1,set_2);

    return combine_sets;
}


void Analysis::Community_Greenhouse_gas(){
    /* THis function conducts overview Analysis on the Community Greenhouse Gas CSV
     * */
    data_processor comm_greenhouse_gas;
    visualization visualizer;

    //COMMUNITY GREENHOUSE GAS DATA TYPES
    std::map<std::string, std::string> COL_TYPES = {
            {"Year", "double"},
            {"GHG Emissions (mt CO2e)", "double"}
    };

    comm_greenhouse_gas.read_data("../data/community-greenhouse-gas.csv", COL_TYPES);

    //LOCAL GOVERNMENT GREENHOUSE GAS
    data_processor local_gov_greenhouse_gas;
    std::map<std::string, std::string> COL_TYPES_1 = {
            {"Year", "double"},
            {"GHG Emissions (t CO2e)", "double"}
    };

    std::map<std::string, std::pair<std::string,std::variant<int, double, std::string, std::optional<int>,std::optional<double>,std::optional<std::string>>>> REPLACEMENTS = {
            {"GHG Emissions (t CO2e)", {"-", std::optional<double>{}}}
    };

    local_gov_greenhouse_gas.read_data("../data/local-government-operations-greenhouse-gas.csv", COL_TYPES_1, REPLACEMENTS);

    //local_gov_greenhouse_gas.print_data();

    //LOCAL GOVERNMENT OPERATIONS FUELS
    data_processor local_gov_fuels;
    std::map<std::string, std::string> COL_TYPE_FUELS = {
            {"Year", "double"},
            {"Quantity", "double"}
    };
    std::map<std::string, std::pair<std::string,std::variant<int, double, std::string, std::optional<int>,std::optional<double>,std::optional<std::string>>>> REP_FUELS = {
            {"Quantity", {"-", std::optional<double>{}}}
    };

    local_gov_fuels.read_data("../data/local-government-operations-fuels.csv", COL_TYPE_FUELS, REP_FUELS);

    data_processor filter_sector = local_gov_fuels.filter_data<std::string>("Sector","Buildings", false);




    //HISTOGRAM ANALYSIS
    //step 1: combine dataset with greenhouse gas emissions
    data_processor greenhouse_gas;
    greenhouse_gas = combine_dataset(local_gov_greenhouse_gas,comm_greenhouse_gas);


    //step 2: drop missing values
    greenhouse_gas = greenhouse_gas.filter_data<std::optional<double>>("GHG Emissions (mt CO2e)", std::optional<double>{}, true);
    std::cout << greenhouse_gas.data_map.size();
    //greenhouse_gas.print_data();


    //step 3: data aggregation
    std::vector<std::string> col_names = {"Year","Sector"};
    greenhouse_gas.aggregation(col_names,"GHG Emissions (mt CO2e)");


    //data_processor cilr_data = comm_greenhouse_gas.filter_data("Sector", "Commercial/Industrial/Large Residential");
    //std::vector<double> Commercial_Industrial_Large_Residential = cilr_data.extract_column<double>("GHG Emissions (mt CO2e)");

    //visualizer.histogram(Commercial_Industrial_Large_Residential);
}

