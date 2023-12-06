//
// Created by Krithika Natarajan on 12/4/23.
//

#ifndef C_FINAL_PROJECT_ANALYSIS_H
#define C_FINAL_PROJECT_ANALYSIS_H

#include "data_processor.h"

class Analysis {

private:
    data_processor combine_dataset(data_processor set_1, data_processor set_2);
    void unit_standardization(data_processor& dataset);
public:
    Analysis();
    ~Analysis();
    void emissions_over_time_by_sector();

    void local_government_operation_fuels();

};


#endif //C_FINAL_PROJECT_ANALYSIS_H
