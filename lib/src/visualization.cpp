//
// Created by Krithika Natarajan on 12/3/23.
//

#include "visualization.h"
#include "matplot/matplot.h"
#include <iostream>
#include <string>

void visualization::scatter_plot(std::vector<double> x_col, std::vector<double> y_col, const std::string& title, const std::string& x_label, const std::string& y_label){
    /*This function creates a scatter based two vectors containing x coordinates and y
     * coordinates
     * Parameters:
     * std::vector - x coordinates
     * std::vector - y coordinates
     * Returns:
     * */

    std::cout << "yay" << std::endl;
    matplot::scatter(x_col, y_col);
    matplot::title("Year vs Carbon Emissions");
    matplot::xlabel("Year");
    matplot::ylabel("Emissions (mt CO2e)");

    // Display the plot
    matplot::show();
}


