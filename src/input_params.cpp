#include <input_params.h>
#include <files.h>
#include <iostream>

InputParams::InputParams(double temperature, double gas_specific_gravity, 
                         double specific_heat_capacity_ratio,
                         double time_horizon, int eos, int units, int intervals,
                         int save_csv_output, int steady_state_solve_only,
                         double space_interval, double econ_weight, double max_iterations,
                         int tolerance_exponent, int objective_scale_exponent,
                         int extension_time_intervals,
                         int exit_if_steady_state_check_infeasible) {
    _temperature = temperature;
    _gas_specific_gravity = gas_specific_gravity;
    _specific_heat_capacity_ratio = specific_heat_capacity_ratio;
    _time_horizon = time_horizon;
    _eos = eos;
    _units = units;
    _intervals = intervals;
    _save_csv_output = save_csv_output;
    _steady_state_solve_only = steady_state_solve_only;
    _space_interval = space_interval;
    _econ_weight = econ_weight,
    _max_iterations = max_iterations;
    _tolerance_exponent = tolerance_exponent;
    _objective_scale_exponent = objective_scale_exponent;
    _extension_time_intervals = extension_time_intervals;
    _exit_if_steady_state_check_infeasible = exit_if_steady_state_check_infeasible;
};

double InputParams::get_temperature() const { return _temperature; };
double InputParams::get_gas_specific_gravity() const { return _gas_specific_gravity; };
double InputParams::get_specific_heat_capacity_ratio() const { return _specific_heat_capacity_ratio; };
double InputParams::get_time_horizon() const { return _time_horizon; };
int InputParams::get_eos() const { return _eos; };
int InputParams::get_units() const { return _units; };
int InputParams::save_output_csv() const { return _save_csv_output; };
int InputParams::solve_steady_state_only() const { return _steady_state_solve_only; };
double InputParams::get_space_interval() const { return _space_interval; };
double InputParams::get_econ_weight() const { return _econ_weight; };
double InputParams::get_max_iterations() const { return _max_iterations; };
int InputParams::get_tolerance_exponent() const { return _tolerance_exponent; };
int InputParams::get_objective_scale_exponent() const { return _objective_scale_exponent; };
int InputParams::get_extension_time_intervals() const { return _extension_time_intervals; };
int InputParams::should_exit_if_steady_state_infeasible() const { return _exit_if_steady_state_check_infeasible; };

std::ostream& operator<<(std::ostream& os, const InputParams &ip) {
    os << "input_parameters(" << std::endl <<
    "temperature: " << ip._temperature << std::endl <<
    "gas specific gravity: " << ip._gas_specific_gravity << std::endl <<
    "specfic heat capacity ratio: " << ip._specific_heat_capacity_ratio << std::endl <<
    "time horizon: " << ip._time_horizon << std::endl <<
    "EOS: " << ip._eos << std::endl <<
    "units: " << ip._units << std::endl <<
    "intervals: " << ip._intervals << std::endl <<
    "save csv outputs: " << ip._save_csv_output << std::endl <<
    "steady state solve only: " << ip._steady_state_solve_only << std::endl <<
    "space interval: " << ip._space_interval << std::endl <<
    "econ weight: " << ip._econ_weight << std::endl <<
    "max iterations: " << ip._max_iterations << std::endl <<
    "tolerance exponent: " << ip._tolerance_exponent << std::endl <<
    "objective scale exponent: " << ip._objective_scale_exponent << std::endl <<
    "extension time intervals: " << ip._extension_time_intervals << std::endl <<
    "exit if steady state check infeasible: " << ip._exit_if_steady_state_check_infeasible << ")" << std::endl;
    return os;
};

