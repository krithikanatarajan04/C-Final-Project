//
// Created by Krithika Natarajan on 12/3/23.
//

#include "visualization.h"
#include "matplot/matplot.h"
#include <iostream>
#include <string>

visualization::visualization(){}

visualization::~visualization(){}

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
    matplot::title(title);
    matplot::xlabel(x_label);
    matplot::ylabel(y_label);

    // Display the plot
    matplot::show();
}

void histogram(std::vector<double> data, const std::string& title, const std::string& x_label, const std::string& y_label){
    /* THis function takes  column of data and creates a histogram
     * Parameter:
     * std::vector - data to plot
     * */
    matplot::hist(data);
    matplot::title(title);
    matplot::xlabel(x_label);
    matplot::ylabel(y_label);

    // Display the plot
    matplot::show();
}


