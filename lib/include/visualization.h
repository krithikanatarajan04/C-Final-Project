//
// Created by Krithika Natarajan on 12/3/23.
//

#ifndef C_FINAL_PROJECT_VISUALIZATION_H
#define C_FINAL_PROJECT_VISUALIZATION_H
#include <vector>
#include <string>




class visualization {
private:
public:
    visualization();
    ~visualization();
    void scatter_plot(std::vector<double> x_col, std::vector<double> y_col, const std::string& title = "", const std::string& x_label = "", const std::string& y_label = "");

};


#endif //C_FINAL_PROJECT_VISUALIZATION_H
