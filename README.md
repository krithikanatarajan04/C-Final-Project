# C-Final-Project

## Overview
The purpose of this project is to explore data regarding Boston's environmental practices through 3 files containing information on greenhouse gas emissions over the years in the community and through the local government as well. To explore and visualize this data, there were three steps,file intake into the system, data cleaning and processing, and then the visualization. To do this, two C++ libraries were used, Fast CSV Parser and Matplotplusplus

### File Intake
Using Fast CSV Parser, I traverse through each of the lines of the CSV and read the data into a Vector of std::maps. At each cell of the csv, I convert the value into the designated type, accounting for any variability within that

### Data Cleaning and Manipulating
Many of the cells contained missing values that needed to be addressed. I replace any missing value with std::optional of the type the rest of the column typically contains using a map the user can provide. There are several methods for manipulating the data, including merging two datasets, adding_data, and extracting specific columns.

### Data Visualization
Using the Matplotplusplus library, I have created methods that help display the findings from this data.

## Building and Compiling
The csv.h header file should be included in my files. For everything to compile and build properly, make sure to download a zip of the Matplotplusplus files and save it within the repository.

## VISUALS


## Sources:
ANALYZE BOSTON - https://data.boston.gov/dataset/greenhouse-gas-emissions
Fast CSV Parser -
Matplotplusplus - https://github.com/alandefreitas/matplotplusplus


