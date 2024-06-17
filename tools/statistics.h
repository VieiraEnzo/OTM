#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>
using namespace std;

double calculate_mean(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

double calculate_standard_deviation(const std::vector<double>& v, double mean) {
    double sum = 0.0;
    for(const auto& val : v) {
        sum += (val - mean) * (val - mean);
    }
    return std::sqrt(sum / v.size());
}

double calculate_coefficient_of_variation(double std_dev, double mean) {
    return (std_dev / mean) * 100;
}


void write_solutions(vector<string> &parameters, vector<double> &solutions, vector<double> &times, string result){
    int best_solution = *max_element(solutions.begin(), solutions.end());
    double mean_solution = calculate_mean(solutions);
    double std_dev_solution = calculate_standard_deviation(solutions, mean_solution);
    double cv_solution = calculate_coefficient_of_variation(std_dev_solution, mean_solution);

    double mean_time = calculate_mean(times);
    double std_dev_time = calculate_standard_deviation(times, mean_time);
    double cv_time = calculate_coefficient_of_variation(std_dev_time, mean_time);

    //, std::ios_base::app
    std::ofstream output_file(result, ios_base::app);
    if (output_file.is_open()) {
        output_file << "\n";
        for(auto p : parameters) output_file << p << " ";
        output_file << "\n"; 
        output_file << "Melhor Solução: " << best_solution << std::endl;
        output_file << "Média das Soluções: " << mean_solution << std::endl;
        output_file << "Coeficiente de Variação das Soluções: " << cv_solution << "%" << std::endl;
        output_file << "Média dos Tempos: " << mean_time << std::endl;
        output_file << "Coeficiente de Variação dos Tempos: " << cv_time << "%" << std::endl;
        output_file.close();
        std::cout << "Resultados gravados no arquivo " << result  << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
    }

    return;
}