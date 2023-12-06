#include <iostream>
#include <string>
#include <fstream>

#include "Analysis.h"



int main() {
    Analysis envr_analysis;
    //envr_analysis.emissions_over_time_by_sector();
    envr_analysis.emissions_by_fuels();

    return 0;
}
