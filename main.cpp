#include <iostream>
#include <string>
#include <fstream>

#include "data_processor.h"
#include "matplot/matplot.h"


int main() {
    // Your code here
    data_processor dp;
    std::cout << "hi" << std::endl;
    //specify any columns that need to have a specific type (everything else will be string)
    std::map<std::string, std::string> columnTypes = {
            {"Year (Calendar Year)", "int"},
            {"GHG Emissions (mt CO2e)", "double"}
    };

    dp.read_data("../data/community-greenhouse-gas.csv", columnTypes);

    //dp.print_data();
    dp.viz_1();







    return 0;
}
