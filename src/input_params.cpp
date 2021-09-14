#include <files.h>
#include <input_params.h>
#include <iostream>

InputParams::InputParams(double temperature, double gas_specific_gravity,
                         double specific_heat_capacity_ratio, int units,
                         double econ_weight, double max_iterations,
                         int tolerance_exponent, int objective_scale_exponent,
                         std::string linear_solver, std::string model_type) {
    _temperature = temperature;
    _gas_specific_gravity = gas_specific_gravity;
    _specific_heat_capacity_ratio = specific_heat_capacity_ratio;
    _units = units;
    _econ_weight = econ_weight, _max_iterations = max_iterations;
    _tolerance_exponent = tolerance_exponent;
    _objective_scale_exponent = objective_scale_exponent;
    _linear_solver = linear_solver;
    _model_type = model_type;
};

double InputParams::get_temperature() const { return _temperature; };
double InputParams::get_gas_specific_gravity() const {
    return _gas_specific_gravity;
};
double InputParams::get_specific_heat_capacity_ratio() const {
    return _specific_heat_capacity_ratio;
};
int InputParams::get_units() const { return _units; };
double InputParams::get_econ_weight() const { return _econ_weight; };
double InputParams::get_max_iterations() const { return _max_iterations; };
int InputParams::get_tolerance_exponent() const { return _tolerance_exponent; };
int InputParams::get_objective_scale_exponent() const {
    return _objective_scale_exponent;
};
std::string InputParams::get_linear_solver() const { return _linear_solver; };
std::string InputParams::get_model_type() const { return _model_type; };

std::ostream &operator<<(std::ostream &os, const InputParams &ip) {
    os << "input_parameters(" << std::endl
       << "temperature: " << ip._temperature << std::endl
       << "gas specific gravity: " << ip._gas_specific_gravity << std::endl
       << "specfic heat capacity ratio: " << ip._specific_heat_capacity_ratio
       << std::endl
       << "units: " << ip._units << std::endl
       << "econ weight: " << ip._econ_weight << std::endl
       << "max iterations: " << ip._max_iterations << std::endl
       << "tolerance exponent: " << ip._tolerance_exponent << std::endl
       << "objective scale exponent: " << ip._objective_scale_exponent << std::endl
       << "model type: " << ip._model_type << ")"
       << std::endl;
    return os;
};
