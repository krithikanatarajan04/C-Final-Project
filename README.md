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

CMake Version Requirement:
The project requires at least CMake version 3.20.
Project Configuration:
C++17 standard is set for the project.

Executable Target:
The source files main.cpp, data_processor.cpp, visualization.cpp, and Analysis.cpp, along with their corresponding header files, are included in the executable.
Matplot++ Library:
The project includes the matplotplusplus-master directory, presumably containing the Matplot++ library for plotting.

Threading Library:
There is a preference set for the pthread library, and the Threads package is required.
The Threads library is linked with your project.
Linking Libraries:

The matplot and Threads::Threads libraries are linked to your project.
TO run the visualization, simply run main and the graph and final table used will appear. 


## Run Analysis
To run this program, you will need to download a zip file from Matplotplusplus:https://github.com/alandefreitas/matplotplusplus,
and store in the repository directory at the same level as the Cmake.txt. Additionally, if you do not have gnuplot installed, you will need to install that on terminal. 
The analyis code is contained within the Analysis.h and Analysis.cpp file but to run the program you will just need to run main and the final table of data used for the visualization will appear along with the visualization. 


## Sources:
ANALYZE BOSTON - https://data.boston.gov/dataset/greenhouse-gas-emissions
Fast CSV Parser - https://github.com/ben-strasser/fast-cpp-csv-parser
Matplotplusplus - https://github.com/alandefreitas/matplotplusplus


