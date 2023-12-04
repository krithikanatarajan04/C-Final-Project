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

    comm_greenhouse_gas.read_data("../data/community-greenhouse-gas.csv", COL_TYPES);

    //HISTOGRAM ANALYSIS
    std::vector<double> Commercial_Industrial_Large_Residential = (comm_greenhouse_gas.filter_data("Sector", "Commercial/Industrial/Large Residential")).extract_column<double>("Sector");

    visualizer.histogram(Commercial_Industrial_Large_Residential);
}