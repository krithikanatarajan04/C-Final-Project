#include <iostream>
#include <string>
#include <fstream>

#include "data_processor.h"
#include "visualization.h"
#include "matplot/matplot.h"


int main() {
    //This is for conducting the analysis
    data_processor comm_greenhouse_gas;
    visualization visualizer;

    //COMMUNITY GREENHOUSE GAS
    std::map<std::string, std::string> columnTypes = {
            {"Year (Calendar Year)", "double"},
            {"GHG Emissions (mt CO2e)", "double"}
    };

    comm_greenhouse_gas.read_data("../data/community-greenhouse-gas.csv", columnTypes);




    return 0;
}
