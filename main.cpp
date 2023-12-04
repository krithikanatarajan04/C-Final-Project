#include <iostream>
#include <string>
#include <fstream>

#include "data_processor.h"
#include "visualization.h"
#include "matplot/matplot.h"


int main() {
    // Your code here
    data_processor dp;
    visualization visualizer;
    std::cout << "hi" << std::endl;
    //specify any columns that need to have a specific type (everything else will be string)
    std::map<std::string, std::string> columnTypes = {
            {"Year (Calendar Year)", "double"},
            {"GHG Emissions (mt CO2e)", "double"}
    };

    dp.read_data("../data/community-greenhouse-gas.csv", columnTypes);

    //std::vector<double> year = dp.extract_column<double>("Year (Calendar Year)");
    //std::vector<double> emissions = dp.extract_column<double>("GHG Emissions (mt CO2e)");
    //visualizer.scatter_plot(year,emissions);

    std::vector<std::map<std::string, std::variant<int, double, std::string>>> sector_filter = dp.filter_data("Sector", "Small Residential");
    data_processor sfd(sector_filter);
    std::vector<double> year = sfd.extract_column<double>("Year (Calendar Year)");
    std::vector<double> emissions = sfd.extract_column<double>("GHG Emissions (mt CO2e)");
    visualizer.scatter_plot(year,emissions);

    //dp.print_data();



    return 0;
}
