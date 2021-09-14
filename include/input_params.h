#ifndef INPUT_PARAMS_H
#define INPUT_PARAMS_H

#pragma once
#include <ostream>
#include <string>

class InputParams {
  private:
    double _temperature;
    double _gas_specific_gravity;
    double _specific_heat_capacity_ratio;
    int _units;
    double _econ_weight;
    double _max_iterations;
    int _tolerance_exponent;
    int _objective_scale_exponent;
    std::string _linear_solver;
    std::string _model_type;

  public:
    InputParams(double temperature, double gas_specific_gravity,
                double specific_heat_capacity_ratio, int units,
                double econ_weight, double max_iterations,
                int tolerance_exponent, int objective_scale_exponent,
                std::string linear_solver, std::string _model_type);

    double get_temperature() const;
    double get_gas_specific_gravity() const;
    double get_specific_heat_capacity_ratio() const;
    int get_units() const;
    double get_econ_weight() const;
    double get_max_iterations() const;
    int get_tolerance_exponent() const;
    int get_objective_scale_exponent() const;
    std::string get_linear_solver() const;
    std::string get_model_type() const;

    friend std::ostream &operator<<(std::ostream &os, const InputParams &ip);
};

InputParams build_input_params(std::string data_path, std::string case_name,
                               std::string data_format = "csv",
                               std::string linear_solver = "mumps",
                               std::string model_type = "ideal");

#endif
