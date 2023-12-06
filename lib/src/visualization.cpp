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

    matplot::scatter(x_col, y_col);
    matplot::title(title);
    matplot::xlabel(x_label);
    matplot::ylabel(y_label);

    // Display the plot
    matplot::show();
}

void visualization::histogram(std::vector<double> data, const std::string& title, const std::string& x_label, const std::string& y_label){
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

void visualization::bar_chart(std::vector<double> x , std::vector<double> y,const std::string& title, const std::string& x_label, const std::string& y_label, std::vector<std::string> category){
    /* This function takes x and y datapoints and displays a bar chart
     * x - represents years
     * y - represents the values
     * categories - buckets for colors
     * */
    //generate color map

}

void visualization::time_series(
        const std::unordered_map<std::string, std::vector<double>>& data,
        const std::vector<double>& x_values,
        const std::vector<std::string>& labels,
        const std::string& title,
        const std::string& x_label,
        const std::string& y_label
)
{
    //traverse through each label
    matplot::hold(matplot::on);
    for (const auto& [key, value] : data) {
        auto line = matplot::plot(x_values, value);
        line->display_name(key); // Use the label for the legend
        line->line_width(2);
    }
    matplot::hold(matplot::off);

    matplot::title(title);
    matplot::xlabel(x_label);
    matplot::ylabel(y_label);
    matplot::legend();
    matplot::show();
}


