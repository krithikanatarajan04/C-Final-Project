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

    //comm_greenhouse_gas.read_data("../data/community-greenhouse-gas.csv", COL_TYPES);

    //LOCAL GOVERNMENT GREENHOUSE GAS
    data_processor local_gov_greenhouse_gas;
    std::map<std::string, std::string> COL_TYPES_1 = {
            {"Year (Fiscal Year)", "double"},
            {"GHG Emissions (t CO2e)", "double"}
    };

    std::map<std::string, std::pair<std::string,std::variant<int, double, std::string, std::optional<int>,std::optional<double>,std::optional<std::string>>>> REPLACEMENTS = {
            {"GHG Emissions (t CO2e)", {"-", std::optional<double>{}}}
    };

    //local_gov_greenhouse_gas.read_data("../data/local-government-operations-greenhouse-gas.csv", COL_TYPES_1, REPLACEMENTS);

    //local_gov_greenhouse_gas.print_data();

    //LOCAL GOVERNMENT OPERATIONS FUELS
    data_processor local_gov_fuels;
    std::map<std::string, std::string> COL_TYPE_FUELS = {
            {"Year (Fiscal Year)", "double"},
            {"Quantity", "double"}
    };
    std::map<std::string, std::pair<std::string,std::variant<int, double, std::string, std::optional<int>,std::optional<double>,std::optional<std::string>>>> REP_FUELS = {
            {"Quantity", {"-", std::optional<double>{}}}
    };

    local_gov_fuels.read_data("../data/local-government-operations-fuels.csv", COL_TYPE_FUELS, REP_FUELS);
    //local_gov_fuels.print_data();
    data_processor filter_sector = local_gov_fuels.filter_data<std::string>("Sector","Buildings", false);
    filter_sector.print_data();

    //COMBINE DATASETS USING SIMILAR HEADERS



    //HISTOGRAM ANALYSIS
    //data_processor cilr_data = comm_greenhouse_gas.filter_data("Sector", "Commercial/Industrial/Large Residential");
    //std::vector<double> Commercial_Industrial_Large_Residential = cilr_data.extract_column<double>("GHG Emissions (mt CO2e)");

    //visualizer.histogram(Commercial_Industrial_Large_Residential);

}

